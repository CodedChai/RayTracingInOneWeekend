public class camera {
	
	public Vec3 lower_left_corner;
	public Vec3 horizontal;
	public Vec3 vertical;
	public Vec3 origin;
	
	camera(){
		lower_left_corner = new Vec3(-2.0f, -1.0f, -1.0f);
		horizontal = new Vec3(4.0f, 0.0f, 0.0f);
		vertical = new Vec3(0.0f, 2.0f, 0.0f);
		origin = new Vec3(0.0f, 0.0f, 0.0f);
	}

	public ray get_ray(float u, float v) {
		// lower left corner + u * horizontal _ v * vertical

		return new ray(origin, horizontal.mul(u).add(vertical.mul(v).sub(origin)).add(lower_left_corner));
	}
	
}
