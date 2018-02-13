
public class Vec3 {
	private float x;
	private float y;
	private float z;


   public Vec3(float x, float y, float z){
        this.x = x;
        this.y = y;
        this.z = z;
    }
	
   public float x() {
		return x;
	}
	
	public float y() {
		return y;
	}
	
	public float z() {
		return z;
	}
	
	public float r() {
		return x;
	}
	
	public float g() {
		return y;
	}
	
	public float b() {
		return z;
	}
	
    public float length(){
        return (float)Math.sqrt(x * x + y * y + z * z);
    }
    
    public float squared_length(){
        return (float)(x * x + y * y + z * z);
    }

    public float dot(Vec3 right){
        return x * right.x() + y * right.y() + z * right.z();
    }

    public Vec3 normalize(){
        float length = length();

        return new Vec3(x / length, y / length, z / length);
    }

    public Vec3 cross(Vec3 right){
        float x_ = y * right.z() - z * right.y();
        float y_ = z * right.x() - x * right.z();
        float z_ = x * right.y() - y * right.x();

        return new Vec3(x_, y_, z_);
    }
    
    public Vec3 make_unit_vector() {
    	float k = 1.0f / (float)Math.sqrt(x * x + y * y + z * z);
    	return new Vec3(x *= k, y *= k, z *= k);
    }
    
    public static Vec3 unit_vector(Vec3 vec) {
    	float len = vec.length();
    	return new Vec3((float)(vec.x() / len),(float)(vec.y() / len), (float)(vec.z() / len));
    }
    
    public Vec3 Abs()
    {
        return new Vec3(Math.abs(x), Math.abs(y), Math.abs(z));
    }

    public Vec3 add(Vec3 right){
        return new Vec3(x + right.x(), y + right.y(), z + right.z());
    }

    public Vec3 add(float right){
        return new Vec3(x + right, y + right, z + right);
    }

    public Vec3 sub(Vec3 right){
        return new Vec3(x - right.x(), y - right.y(), z - right.z());
    }

    public Vec3 sub(float right){
        return new Vec3(x - right, y - right, z - right);
    }

    public Vec3 mul(Vec3 right){
        return new Vec3(x * right.x(), y * right.y(), z * right.z());
    }

    public Vec3 mul(float right){
        return new Vec3(x * right, y * right, z * right);
    }

    public Vec3 div(Vec3 right){
        return new Vec3(x / right.x(), y / right.y(), z / right.z());
    }

    public Vec3 div(float right){
        return new Vec3(x / right, y / right, z / right);
    }

    public float max(){
        return Math.max(x, Math.max(y, z));
    }
    
    public static Vec3 zero() {
    	return new Vec3(0.0f, 0.0f, 0.0f);
    }
    
    public String toString(){
        return "(" + x + ", " + y + ", " + z + ")";
    }
}
