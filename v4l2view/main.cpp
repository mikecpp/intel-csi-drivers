#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <errno.h>

#include <opencv2/opencv.hpp>

using namespace cv;

struct plane_start {
	void * start;
};

struct buffer {
	struct plane_start* plane_start;
	struct v4l2_plane* planes_buffer;
};

#define V4L2_DEVICE     "/dev/video1"

int main(int argc, char **argv)
{
	int fd;
	fd_set fds;
	FILE *file_fd;
	struct timeval tv;
	int ret = -1, i, j, r;
	int num_planes;
	struct v4l2_capability cap;
	struct v4l2_format fmt;
	struct v4l2_requestbuffers req;
	struct v4l2_buffer buf;
	struct v4l2_plane* planes_buffer;
	struct plane_start* plane_start;
	struct buffer *buffers;
	enum v4l2_buf_type type;

	fd = open(V4L2_DEVICE, O_RDWR);

	if (fd < 0) {
		printf("open device: %s fail\n", V4L2_DEVICE);
		return -1;
	}

	if (ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
		printf("Get video capability error!\n");
		return -1;
	}

	if (!(cap.device_caps & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
		printf("Video device not support capture!\n");
		return -1;
	}

	memset(&fmt, 0, sizeof(struct v4l2_format));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	fmt.fmt.pix_mp.width       = 1920;
	fmt.fmt.pix_mp.height      = 1280;
	fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_YUYV;
	fmt.fmt.pix_mp.field       = V4L2_FIELD_ANY;

	if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
		printf("Set format fail\n");
		return -1;
	}

	req.count  = 5;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	req.memory = V4L2_MEMORY_MMAP;
	if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0) {
		printf("Reqbufs fail\n");
		return -1;
	}

	num_planes = fmt.fmt.pix_mp.num_planes;
    printf("num_planes = %d\n", num_planes);

	buffers = (struct buffer*) malloc(req.count * sizeof(*buffers));

	for(i = 0; i < req.count; i++) {
		memset(&buf, 0, sizeof(buf));
		planes_buffer = (struct v4l2_plane*)  calloc(num_planes, sizeof(*planes_buffer));
		plane_start   = (struct plane_start*) calloc(num_planes, sizeof(*plane_start));

		memset(planes_buffer, 0, sizeof(*planes_buffer));
		buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		buf.memory   = V4L2_MEMORY_MMAP;
		buf.m.planes = planes_buffer;
		buf.length   = num_planes;
		buf.index    = i;
		if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) {
			printf("Querybuf fail\n");
			return -1;
		}

		(buffers + i)->planes_buffer = planes_buffer;
		(buffers + i)->plane_start = plane_start;
		for(j = 0; j < num_planes; j++) {
			printf("plane[%d]: length = %d\n", j, (planes_buffer + j)->length);
			printf("plane[%d]: offset = %d\n", j, (planes_buffer + j)->m.mem_offset);
			(plane_start + j)->start = mmap (NULL, (planes_buffer + j)->length, PROT_READ | PROT_WRITE,
									         MAP_SHARED, fd, (planes_buffer + j)->m.mem_offset);
			if (MAP_FAILED == (plane_start +j)->start) {
				printf ("mmap failed\n");
				return -1;
			}
		}
	}

	for (i = 0; i < req.count; ++i) {
		memset(&buf, 0, sizeof(buf));

		buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		buf.memory      = V4L2_MEMORY_MMAP;
		buf.length   	= num_planes;
		buf.index       = i;
		buf.m.planes 	= (buffers + i)->planes_buffer;

		if (ioctl (fd, VIDIOC_QBUF, &buf) < 0)
			printf ("VIDIOC_QBUF failed\n");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

	if (ioctl(fd, VIDIOC_STREAMON, &type) < 0)
		printf ("VIDIOC_STREAMON failed\n");

	int num = 0;
	struct v4l2_plane *tmp_plane;
	tmp_plane = (struct v4l2_plane*) calloc(num_planes, sizeof(*tmp_plane));

    Mat matRGB;
    int count = 0;	
	while (1) {
        printf("count = %d\n", count++);
		memset(&buf, 0, sizeof(buf));
		buf.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		buf.memory   = V4L2_MEMORY_MMAP;
		buf.m.planes = tmp_plane;
		buf.length   = num_planes;
		if (ioctl (fd, VIDIOC_DQBUF, &buf) < 0)
			printf("dqbuf fail\n");

        Mat matYUV(1280, 1920, CV_8UC2, ((buffers + buf.index)->plane_start)->start);

        cvtColor(matYUV, matRGB, COLOR_YUV2BGR_UYVY);
        namedWindow("window_color", WINDOW_AUTOSIZE);
        imshow("window_color", matRGB);

		waitKey(33);

		if (ioctl (fd, VIDIOC_QBUF, &buf) < 0)
			printf("failture VIDIOC_QBUF\n");
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	if (ioctl(fd, VIDIOC_STREAMOFF, &type) < 0)
		printf("VIDIOC_STREAMOFF fail\n");

	free(tmp_plane);

	ret = 0;

	return ret;
}
