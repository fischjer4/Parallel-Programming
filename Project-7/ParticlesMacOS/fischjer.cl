typedef float4 point;
typedef float4 vector;
typedef float4 color;
typedef float4 sphere;


vector changeColor(color cp,float r, float g, float b, float a) {
    cp.x = r;
    cp.y = g;
    cp.z = b;
    return cp;
}

vector
Bounce( vector in, vector n )
{
	vector out = in - (float4)2.*n*dot(in.xyz, n.xyz);
	out.w = 0.;
	return out;
}

vector
BounceSphere( point p, vector in, sphere s )
{
	vector n;
	n.xyz = fast_normalize( p.xyz - s.xyz );
	n.w = 0.;
	return Bounce( in, n );
}

bool
IsInsideSphere( point p, sphere s )
{
	float r = fast_length( p.xyz - s.xyz );
	return  ( r < s.w );
}

kernel
void
Particle( global point *dPobj, global vector *dVel, global color *dCobj )
{
	const float4 G       = (float4) ( 0., -9.8, 0., 0. );
	const float  DT      = 0.1;
    const sphere Sphere1 = (sphere)( -300., -800., 0.,  300. );
    const sphere Sphere2 = (sphere)(500., -800., 0., 300.);

    
	int gid = get_global_id( 0 );

	point  p = dPobj[gid];
	vector v = dVel[gid];
    color  c = dCobj[gid];

	point  pp = p + v*DT + (float4)(.5*DT*DT)*G;
	vector vp = v + G*DT;
	pp.w = 1.;
	vp.w = 0.;

	if( IsInsideSphere( pp, Sphere1 ) )
	{
		vp = BounceSphere( p, v, Sphere1 );
		pp = p + vp*DT + (float4)(.5*DT*DT)*G;
        c = changeColor(c, 0.0, 1.0, 0.5091, 1.0);
	}
    if( IsInsideSphere( pp, Sphere2 ) )
    {
        vp = BounceSphere( p, v, Sphere2 );
        pp = p + vp*DT + (float4)(.5*DT*DT)*G;
        c = changeColor(c, .2f, 1.0f, 1.0f, 1.0);
    }


	dPobj[gid] = pp;
	dVel[gid]  = vp;
    dCobj[gid] = c;
}
