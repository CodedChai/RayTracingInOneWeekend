public class sphere extends hitable{
	private Vec3 center;
	private float radius;
	
	sphere(Vec3 center, float radius, material mat){
		this.center = center;
		this.radius = radius;
		this.mat = mat;
	}

	@Override
	public hit_record hit(ray r, float t_min, float t_max, hit_record rec) {
		Vec3 oc = r.origin().sub(center);
		float a = r.direction().dot(r.direction());
		float b = oc.dot(r.direction());
		float c = oc.dot(oc) - radius * radius;
		float discriminant = b*b - a*c;
		// if discriminant is greater than 0 check quad formula
		if(discriminant > 0) {
			float temp = (float) ((-b - Math.sqrt(b*b-a*c))/a);
			if(temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(temp);
				// (p - center) / radius
				rec.normal = rec.p.sub(center).div(radius);
				rec.valid = true;
				return rec;
			}
			temp = (float) ((-b + Math.sqrt(b*b - a*c))/a);
			if(temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(temp);
				// (p - center) / radius
				rec.normal = rec.p.sub(center).div(radius);
				rec.valid = true;
				return rec;
			}
		}
		rec.valid = false;
		return rec;
	}
	
	
}
