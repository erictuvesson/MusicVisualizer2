// https://github.com/MonoGame/MonoGame/blob/develop/MonoGame.Framework/MathHelper.cs

//--- Math Helper ---

#define Pi			3.14159265359
#define PiOver2		(Pi / 2.0)
#define PiOver4		(Pi / 4.0)
#define TwoPi		(Pi * 2.0)

#define TAU 6.28318530718

vec2 screen_center() {
	vec2 res = iResolution.xy / iResolution.y;
	return (gl_FragCoord.xy / iResolution.y) - res / 2.0;
}

//-----------------
