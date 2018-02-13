
public abstract class material {
	Vec3 albedo;

	abstract ray scatter(ray r_in, hit_record rec);
	
}
