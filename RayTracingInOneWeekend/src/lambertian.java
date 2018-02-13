
public class lambertian extends material {
	
	lambertian(Vec3 a){
		albedo = a;
	}
	
	@Override
	ray scatter(ray r_in, hit_record rec) {
		Vec3 target = rec.p.add(rec.normal).add(main.random_in_unit_sphere());
		return new ray(rec.p, target.sub(rec.p));
	}

}
