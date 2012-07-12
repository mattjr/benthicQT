varying vec3 screennorm;
varying float zheight;
uniform vec2 zrange;
vec3 HSV_to_RGB (vec3 hsv){
  vec3 color;
  float f,p,q,t;
  float h,s,v;
  float r=0.0,g=0.0,b=0.0;
  float i;
  if (hsv[1] == 0.0){
    if (hsv[2] != 0.0){
      color.x = hsv[2];
    }
  }
  else{
    h = hsv.x * 360.0;
    s = hsv.y;
    v = hsv.z;
    if (h == 360.0) {
      h=0.0;
    }
    h /=60.0;
    i = floor (h);
    f = h-i;
    p = v * (1.0 - s);
    q = v * (1.0 - (s * f));
    t = v * (1.0 - (s * (1.0 -f)));
    if (i == 0.0){
      r = v;
      g = t;
      b = p;
    }
    else if (i == 1.0){
      r = q;
      g = v;
      b = p;
    }
    else if (i == 2.0){
      r = p;
      g = v;
      b = t;
    }
    else if (i == 3.0) {
      r = p;
      g = q;
      b = v;
    }
    else if (i == 4.0) {
      r = t;
      g = p;
      b = v;
    }
    else if (i == 5.0) {
      r = v;
      g = p;
      b = q;
    }
    color.r = r;
    color.g = g;
    color.b = b;
  }
  return color;
}

vec3 rainbowColorMap(float hue) {
  return HSV_to_RGB(vec3(hue, 1.0, 1.0));
}

void main(void)
{
	highp vec3 N= normalize(screennorm);
    highp vec3 L = normalize(vec3(1,  2.5,  -1));
    highp vec3 E = vec3(0, 0, 1);
    highp vec3 H = normalize(L + E);
   // float Shininess=64.0;
    highp float df = max(0.0, dot(N, L));
   // highp float sf = max(0.0, dot(N, H));
   // sf = pow(sf, Shininess);
	vec3 AmbientMaterial = vec3(0.05, 0.05, 0.05);
	float val = (zheight -zrange.x )/ (zrange.y -zrange.x);
	vec3 Diffuse =rainbowColorMap(val);
	
	//vec3 Diffuse = vec3(zheight/4.0, 0.00, 0.00);
	//vec3 SpecularMaterial = vec3(0.5, 0.5, 0.5);
    lowp vec3 color = AmbientMaterial + df * Diffuse; //+ sf * SpecularMaterial;

		gl_FragColor	= vec4(color,1);
}