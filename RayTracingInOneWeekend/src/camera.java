public class camera {
	
	public Vec3 lower_left_corner;
	public Vec3 horizontal;
	public Vec3 vertical;
	public Vec3 origin;
	
	// vfov is the vertical FoV, top to bottom in degrees
	camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect){
		Vec3 u, v, w;
		origin = lookfrom;
		w = lookfrom.sub(lookat).normalize();
		u = vup.cross(w).normalize();
		v = w.cross(u);
		float theta = (float) (vfov * Math.PI / 180f);
		float half_height = (float) Math.tan(theta/2f);
		float half_width = aspect * half_height;
		lower_left_corner = origin.sub(u.mul(half_width)).sub(v.mul(half_height)).sub(w);
		horizontal = u.mul(2f * half_width);
		vertical = v.mul(2f * half_height);
	}

	public ray get_ray(float u, float v) {
		// lower left corner + u * horizontal _ v * vertical

		return new ray(origin, horizontal.mul(u).add(vertical.mul(v).sub(origin)).add(lower_left_corner));
	}
	
}
