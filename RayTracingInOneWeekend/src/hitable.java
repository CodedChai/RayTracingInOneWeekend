

public abstract class hitable {
	public material mat;
	
	public abstract hit_record hit(ray r, float t_min, float t_max, hit_record rec);
	
}
