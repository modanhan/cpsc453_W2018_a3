// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage
in vec3 Colour;
in vec2 UV;


// first output is mapped to the framebuffer's colour index by default
out vec4 FragmentColour;

uniform sampler2D ourTexture;
uniform sampler2D borderTexture;
uniform int mode;
uniform int filt;
uniform int w;
uniform int h;
uniform int gSize;
uniform int hori;
uniform int level;


const float e = 2.71828182845904523536028747135266249775724709369995957;
const float pi = 3.141592653589793238462643383279502884197169;
float gaussian2D(float sigma, float x, float y) {
//	float sigma = 1.0f;
	float exponent = (x*x+y*y)/(2*sigma*sigma);
	return 1/(pow(e,exponent)*(2*pi*sigma*sigma));
	//return 0;
}
float gaussian1D(float sigma, float x) {
//	float sigma = 1.0f;
	float exponent = (x*x)/(2*sigma*sigma);
	return 1/(pow(e,exponent)*sqrt(2*pi*sigma*sigma));
	//return 0;
}
void main(void)
{
	
    // write colour output without modification
   FragmentColour = texture(ourTexture, UV);
    if (mode == 0) {
		if (filt == 1) 
			FragmentColour = 
				vec4(texture(ourTexture,UV).r/3.0f +texture(ourTexture,UV).g/3.0f +texture(ourTexture,UV).b/3.0);
		else if (filt == 2)
			FragmentColour = 
				vec4(texture(ourTexture,UV).r*.299f +texture(ourTexture,UV).g*.587f +texture(ourTexture,UV).b*.114f);
		else if (filt == 3)
			FragmentColour =	
				vec4(texture(ourTexture,UV).r*.213f +texture(ourTexture,UV).g*.715f +texture(ourTexture,UV).b*.072f);
		else if (filt == 4)
			FragmentColour =
				vec4(1.0f) - texture(ourTexture,UV);
		else if (filt == 5) {
			float intensity = texture(borderTexture,UV).r +texture(borderTexture,UV).g +texture(borderTexture,UV).b;
			if (intensity == 0)
				FragmentColour = vec4(1.0f)-texture(ourTexture, UV);
			else
				FragmentColour = texture(borderTexture, UV);
		}
	}
	else if (mode == 1) {
		if (filt == 1) {
			FragmentColour = /*FragmentColour + */
				texture(ourTexture,UV-vec2(-1.0f/w, -1.0f/h)) +
				2.0f*texture(ourTexture, UV-vec2(-1.0f/w, 0.0f)) +
				texture(ourTexture, UV-vec2(-1.0f/w, 1.0f/h)) -
				texture(ourTexture,UV-vec2(1.0f/w, -1.0f/h)) -
				2.0f*texture(ourTexture, UV-vec2(1.0f/w, 0.0f)) -
				texture(ourTexture, UV-vec2(1.0f/w, 1.0f/h));
		}
		else if (filt == 2) {
			FragmentColour = 
				texture(ourTexture,UV-vec2(-1.0f/w, -1.0f/h)) +
				2.0f*texture(ourTexture, UV-vec2(0.0f, -1.0f/h)) +
				texture(ourTexture, UV-vec2(1.0f/w, -1.0f/h)) -
				texture(ourTexture,UV-vec2(-1.0f/w, 1.0f/h)) -
				2.0f*texture(ourTexture, UV-vec2(0.0f, 1.0f/h)) -
				texture(ourTexture, UV-vec2(1.0f/w, 1.0f/h));
		}
		else if (filt == 3) {
			FragmentColour = 
				5*texture(ourTexture,UV-vec2(0.0f, 0.0f)) -
				texture(ourTexture, UV-vec2(0.0f, -1.0f/h)) -
				texture(ourTexture, UV-vec2(1.0f/w, 0.0f)) -
				texture(ourTexture,UV-vec2(-1.0f/w, 0.0f)) -
				texture(ourTexture, UV-vec2(0.0f, 1.0f/h));
		}
		else
			FragmentColour = texture(ourTexture, UV);
	}
	else if (mode == 2) {// || mode == 3) {
		
		if (filt != 0) {
			int bound = (gSize-1)/2;
			float sigma = .15f+0.45*filt;//0.665f+.25f*filt;//+0.2f*filt;//+filt/bound;
			float total = 0;
			vec4 blur = vec4(0);//texture(ourTexture, -UV);//vec4(vec3(.2f,.2f,.2f),0.0f);// = 0*texture(ourTexture, UV);
			for (int i=-bound; i<=bound; i++) {
				for (int j=-bound; j<=bound; j++) {
					//blur = blur + ((1.0f/(2.0f*pi*pow(sigma,2)))*pow(e,-((pow(i*1.0f,2)+pow(j*1.0f,2))/(2.0f*pow(sigma,2)))))*texture(ourTexture, UV-vec2(i*1.0f/w,j*1.0f/h));
					//FragmentColour = FragmentColour + texture(ourTexture,UV-(
					blur = blur + gaussian2D(sigma,i*1.0f,j*1.0f)*texture(ourTexture, UV-vec2((i*1.0f)/w,(j*1.0f)/h));
//					total = gaussian(sigma,i,j);
				}
			}
			FragmentColour = blur;
			
		}
		else
			FragmentColour = texture(ourTexture, UV);
	}
	else if (mode == 3) {
		if (filt!=0) {
			int bound = 3;//(gSize-1)/2;
			float sigma = .15f+0.45*7;//filt;//0.665f+.25f*filt;//+0.2f*filt;//+filt/bound;
			float total = 0;
			vec4 blur = vec4(0);//texture(ourTexture, -UV);//vec4(vec3(.2f,.2f,.2f),0.0f);// = 0*texture(ourTexture, UV);
			for (int i=-bound; i<=bound; i++) {
				//blur = blur + ((1.0f/(2.0f*pi*pow(sigma,2)))*pow(e,-((pow(i*1.0f,2)+pow(j*1.0f,2))/(2.0f*pow(sigma,2)))))*texture(ourTexture, UV-vec2(i*1.0f/w,j*1.0f/h));
				//FragmentColour = FragmentColour + texture(ourTexture,UV-(
				if (hori == 1)
					blur = blur + gaussian1D(sigma,i*1.0f)*texture(ourTexture, UV-vec2((i*1.0f)/w,0.0f/h));
				else
					blur = blur + gaussian1D(sigma,i*1.0f)*texture(ourTexture, UV-vec2(0.0f/w,(i*1.0f)/h));
		//					total = gaussian(sigma,i,j);
			}
		//			if (total < 1.0f)
		//				FragmentColour = vec4(Colour,0.0);
			FragmentColour = blur;
//			FragmentColour = texture(ourTexture, UV)+vec4(0.5);
		}
		else
			FragmentColour = texture(ourTexture, UV);
	}
	else{
		FragmentColour = texture(ourTexture, UV);
	}
}
