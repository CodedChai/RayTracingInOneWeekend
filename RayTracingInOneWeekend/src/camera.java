import java.util.Random;

public class camera {
	Random rand = new Random();
	public Vec3 lower_left_corner;
	public Vec3 horizontal;
	public Vec3 vertical;
	public Vec3 origin;
	public float lens_radius;
	public Vec3 u, v, w;
	
	Vec3 random_in_unit_disk() {
		Vec3 p;
		do {
			p = new Vec3(rand.nextFloat(), rand.nextFloat(), 0f).sub(new Vec3(1f, 1f, 0f)).mul(2f);
		} while(p.dot(p) >= 1.0f);
		return p;
	}

	// vfov is the vertical FoV, top to bottom in degrees
	camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist){
		lens_radius = aperture / 2f;
		origin = lookfrom;
		w = lookfrom.sub(lookat).normalize();
		u = vup.cross(w).normalize();
		v = w.cross(u);
		float theta = (float) (vfov * Math.PI / 180f);
		float half_height = (float) Math.tan(theta/2f);
		float half_width = aspect * half_height;
		lower_left_corner = origin.sub(u.mul(half_width * focus_dist)).sub(v.mul(half_height * focus_dist)).sub(w.mul(focus_dist));
		horizontal = u.mul(2f * half_width * focus_dist);
		vertical = v.mul(2f * half_height * focus_dist);
	}

	public ray get_ray(float s, float t) {
		// lower left corner + u * horizontal _ v * vertical
		Vec3 rd = random_in_unit_disk().mul(lens_radius);
		Vec3 offset = u.mul(rd.x()).add(v.mul(rd.y()));
		
		return new ray(origin.add(offset), lower_left_corner.add(horizontal.mul(s)).add(vertical.mul(t)).sub(origin).sub(offset));
	}
	
}
