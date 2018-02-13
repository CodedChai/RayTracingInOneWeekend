
public class ray {
	Vec3 A;
	Vec3 B;
	public boolean valid;
	
	ray(Vec3 a, Vec3 b){
		A = a; B = b; valid = true;
	}
	
	public Vec3 origin() {
		return A;
	}
	
	public Vec3 direction() {
		return B;
	}
	
	public Vec3 point_at_parameter(float t) {
		// A + B * t
		return A.add(B.mul(t));
	}

}
