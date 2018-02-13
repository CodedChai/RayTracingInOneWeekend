
public abstract class material {
	Vec3 attenuation;
	
	abstract ray scatter(ray r_in, hit_record rec);
	
}
