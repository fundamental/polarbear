#ifndef __Filter__
#define __Filter__

class Filter
{
public:
	unsigned char xi;
	unsigned char yi;
	
	double xbuf[8192];
	double ybuf[8192];

	double* a;	
	double* b;

	double gain;
	
	int M;
	int N;

	bool active;

	Filter(double* _a = 0,int _M = 0,double* _b = 0,int _N = 0)
	{
		int i;
		for (i=0;i<8192;i++)
		{
			xbuf[i] = ybuf[i] = 0;
		}
		xi = 0;
		yi = 0;
		
		a = _a;
		M = _M;
		
		b = _b;
		N = _N;
		
		gain = 1;
		
		active = 0;
	}
	
	void Apply(short* in,short* out,int n,int inc)
	{
		int i;
		n = n<<1;
		if (active)
		{
			for (i=0;i<n;i+=inc) {
				double y = 0;
				unsigned char j;
				xbuf[xi] = double(in[i]) * gain;
				j = xi;
				int k;
				for(k=0;k<M;k++){
					y += xbuf[j&1023] * a[k];
					j--;
				}
				j = yi;
				for(k=1;k<N;k++){
					y -= ybuf[j&1023] * b[k];
					j--;
				}
				yi = (yi + 1) & 1023;
				ybuf[yi] = y;

				if (y>32767.) y = 32767.;
				if (y<-32767.) y = -32767.;


				out[i] = (short) y;

				xi = (xi + 1) & 1023;
			}
		}else{
			for (i=0;i<n;i+=inc) {
				xbuf[xi] = 0;
				yi = (yi + 1) & 1023;
				ybuf[yi] = 0;
				out[i] = 0;
				xi = (xi + 1) & 1023;
			}
		}
	}
	void Apply(double* in,double* out,int n)
	{
		int i;
		if (active)
		{
			for (i=0;i<n;i++) {
				double y = 0;
				unsigned char j;
				xbuf[xi] = in[i] * gain;
				j = xi;
				int k;
				for(k=0;k<M;k++){
					y += xbuf[j&1023] * a[k];
					j--;
				}
				j = yi;
				for(k=1;k<N;k++){
					y -= ybuf[j&1023] * b[k];
					j--;
				}
				yi = (yi + 1) & 1023;
				ybuf[yi] = y;

				if (y>1.) y = 1.;
				if (y<-1.) y = -1.;

				out[i] = y;

				xi = (xi + 1) & 1023;
			}
		}else{
			for (i=0;i<n;i++) {
				xbuf[xi] = 0;
				yi = (yi + 1) & 1023;
				ybuf[yi] = 0;
				out[i] = 0;
				xi = (xi + 1) & 1023;
			}
		}
	}
	void Apply(float* in,float* out,int n)
	{
		int i;
		if (active)
		{
			for (i=0;i<n;i++) {
				double y = 0;
				unsigned char j;
				xbuf[xi] = in[i] * gain;
				j = xi;
				int k;
				for(k=0;k<M;k++){
					y += xbuf[j&1023] * a[k];
					j--;
				}
				j = yi;
				for(k=1;k<N;k++){
					y -= ybuf[j&1023] * b[k];
					j--;
				}
				yi = (yi + 1) & 1023;
				ybuf[yi] = y;

				if (y>1.) y = 1.;
				if (y<-1.) y = -1.;

				out[i] = y;

				xi = (xi + 1) & 1023;
			}
		}else{
			for (i=0;i<n;i++) {
				xbuf[xi] = 0;
				yi = (yi + 1) & 1023;
				ybuf[yi] = 0;
				out[i] = 0;
				xi = (xi + 1) & 1023;
			}
		}
	}
};

#endif
