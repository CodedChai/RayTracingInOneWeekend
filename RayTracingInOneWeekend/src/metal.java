
public class metal extends material {
	public float fuzz;
	public Vec3 albedo;
	
	metal(Vec3 a, float f){
		albedo = a;
		attenuation = albedo;
		if(f < 1 && f >= 0) {
			fuzz = f;
		} else {
			fuzz = 1;
		}
	}
	
	metal(Vec3 a){
		albedo = a;
		attenuation = albedo;
		fuzz = 0f;
	}
	
	Vec3 reflect(Vec3 v, Vec3 n) {
		// v - 2 * dot(v, n) * n
		return v.sub(n.mul(v.dot(n) * 2f));
	}

	@Override
	ray scatter(ray r_in, hit_record rec) {
		Vec3 reflected = reflect(r_in.direction().normalize(), rec.normal);
		ray scattered = new ray(rec.p, reflected.add(main.random_in_unit_sphere().mul(fuzz)));
		scattered.valid = (scattered.direction().dot(rec.normal) > 0);
		
		return scattered;
	}
}
