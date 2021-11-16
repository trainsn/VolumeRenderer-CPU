#include<stdio.h>
#include <iostream>
#include<render.h>

using namespace std;

void usage(char* prgm) {
	printf(" usage: %s udim vdim volume colormap alpha beta gamma out \n",
		prgm);
	exit(0);
}

int main(int argc, char* argv[]) {

	if (argc != 9) usage(argv[0]);

	int udim = atoi(argv[1]);
	int vdim = atoi(argv[2]);

	float alpha = (float)atoi(argv[5]);
	float beta = (float)atoi(argv[6]);
	float gamma = (float)atoi(argv[7]);

	int xdim = 256, ydim = 256, zdim = 256;
	long long int size = xdim * ydim;
	size *= zdim;
	printf("%d %d %d\n", xdim, ydim, zdim);
	float *volume = new float[size];

	FILE* fp;
	if (!(fp = fopen(argv[3], "rb"))) {
		cout << "Error: opening .raw file failed" << endl;
		exit(EXIT_FAILURE);
	}
	else {
		cout << "OK: open .raw file successed" << endl;
	}
	if (fread(volume, sizeof(float), size, fp) != size) {
		cout << "Error: read .raw file failed" << endl;
		exit(1);
	}
	else {
		cout << "OK: read .raw file successed" << endl;
	}

	volumeRender vr(xdim, ydim, zdim, udim, vdim, volume);
	float vol_min, vol_max;
	vr.computeVolMinMax(volume, size, vol_min, vol_max);
	cout << vol_min << " " << vol_max << endl;
	vr.readCmapFile(argv[4]);
	vr.set_view(alpha, beta, gamma);
	vr.execute();
	vr.out_to_image(argv[8]);
}
