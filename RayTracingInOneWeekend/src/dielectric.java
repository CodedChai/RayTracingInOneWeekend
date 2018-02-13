import java.util.Random;

public class dielectric extends material{

	public float ref_idx;
	
	Random rand = new Random();
	dielectric(float ri){
		ref_idx = ri;
	}
	
	@Override
	ray scatter(ray r_in, hit_record rec) {
		ray scattered;
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = new Vec3(1.0f, 1.0f, 1.0f);
		Vec3 refracted = null;
		float reflect_prob;
		float cosine;
		// Snell's
		if(r_in.direction().dot(rec.normal) > 0) {
			outward_normal = new Vec3(rec.normal.x() * -1f, rec.normal.y() * -1f, rec.normal.z() * -1f);
			ni_over_nt = ref_idx;
			cosine = ref_idx * r_in.direction().dot(rec.normal) / r_in.direction().length();
		} else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_idx;
			cosine = -1f * r_in.direction().dot(rec.normal) / r_in.direction().length();
		}
		refracted = refract(r_in.direction(), outward_normal, ni_over_nt);
		if(refracted != null) {
			reflect_prob = schlick(cosine, ref_idx);
		} else {
			reflect_prob = 1.0f;
			// return false, this will be the false of checkRefract
		}
		if(rand.nextFloat() < reflect_prob){
			scattered = new ray(rec.p, reflected);
		} else {
			scattered = new ray(rec.p, refracted);
		}
		
		return scattered;
	}
	
	Vec3 refract(Vec3 v, Vec3 n, float ni_over_nt) {
		Vec3 refracted = null;
		Vec3 uv = v.normalize();
		float dt = uv.dot(n);
		float discriminant = 1.0f - ni_over_nt * ni_over_nt * ( 1f - dt * dt );
	
		if(discriminant > 0) {
			refracted = uv.sub(n.mul(dt)).mul(ni_over_nt).sub(n.mul((float)(Math.sqrt(discriminant))));
		}

		return refracted;
	}
	
	Vec3 reflect(Vec3 v, Vec3 n) {
		// v - 2 * dot(v, n) * n
		return v.sub(n.mul(v.dot(n) * 2f));
	}
	
	float schlick(float cosine, float ref_idx) {
		float r0 = (1f-ref_idx) / (1f+ref_idx);
		r0 = r0 * r0;
		return (float) (r0 + (1f - r0)* Math.pow((1f - cosine), 5f));
	}

}
