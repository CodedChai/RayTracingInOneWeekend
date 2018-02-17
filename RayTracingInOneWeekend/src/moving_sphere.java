
public class moving_sphere extends hitable{
	
	private Vec3 center0;
	private Vec3 center1;
	private float radius, time0, time1;
	
	moving_sphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float radius, material mat){
		this.center0 = cen0;
		this.center1 = cen1;
		this.time0 = t0;
		this.time1 = t1;
		this.radius = radius;
		this.mat = mat;
	}

	@Override
	public hit_record hit(ray r, float t_min, float t_max, hit_record rec) {
		Vec3 oc = r.origin().sub(center(r.time()));
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
				rec.normal = rec.p.sub(center(r.time())).div(radius);
				rec.valid = true;
				return rec;
			}
			temp = (float) ((-b + Math.sqrt(b*b - a*c))/a);
			if(temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(temp);
				// (p - center) / radius
				rec.normal = rec.p.sub(center(r.time())).div(radius);
				rec.valid = true;
				return rec;
			}
		}
		rec.valid = false;
		return rec;
	}

	@Override
	public AABB bounding_box(float t0, float t1, AABB box){
		return new AABB(center(t1-t0).sub(new Vec3(radius, radius, radius)),
				center(t1 - t0).add(new Vec3(radius, radius, radius)));

	}

	public AABB surrounding_box(AABB box0, AABB box1){
		Vec3 small = new Vec3(ffmin(box0._min.x(), box1._min.x()),
				ffmin(box0._min.y(), box1._min.y()),
				ffmin(box0._min.z(), box1._min.z()));

		Vec3 big = new Vec3(ffmax(box0._max.x(), box1._max.x()),
				ffmax(box0._max.y(), box1._max.y()),
				ffmax(box0._max.z(), box1._max.z()));

		return new AABB(small, big);
	}

	private float ffmin(float a, float b){
		return a < b ? a : b;
	}

	private float ffmax(float a, float b){
		return a > b ? a : b;
	}

	private Vec3 center(float time) {
		return center0.add(center1.sub(center0).mul((time - time0) / (time1 - time0)));
	}
}
