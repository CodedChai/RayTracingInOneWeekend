import java.io.*;
import java.util.Random;

public class main {

	public static void main(String[] args) {
		System.out.println("Starting render, please wait...");
		long startTime = System.nanoTime();
		
		StringBuilder sb = new StringBuilder();
		int nx = 1920;
		int ny = 1080;
		int ns = 100;
		
		// Create all hitable spheres
		Random rand = new Random();
		hitable[] list = new hitable[2];
		list[0] = new sphere(new Vec3(0.0f, 0.0f, -1.0f),0.5f);
		list[1] = new sphere(new Vec3(0.0f, -100.5f, -1.0f), 100);
		hitable_list world = new hitable_list(list, 2);
		camera cam = new camera();
		
		sb.append("P3\n" + nx + " " + ny + "\n255\n");
		for(int j = ny-1; j >=0; j--) {
			for(int i = 0; i < nx; i++) {
				Vec3 col = Vec3.zero();
				for(int s = 0; s < ns; s++) {
					float u = (float)(i + rand.nextFloat())/(float)(nx);
					float v = (float)(j + rand.nextFloat())/(float)(ny);
					
					ray r = cam.get_ray(u, v);
					Vec3 p = r.point_at_parameter(2.0f);
					// col += color(r, world)
					col = col.add(color(r, world));
				}
				// col /= ns
				col = col.div(ns);
				
				// A basic gamma2 correction approximation. Just SQRT because they're 50% reflectors
				col = new Vec3((float)Math.sqrt(col.r()), (float)Math.sqrt(col.g()), (float)Math.sqrt(col.b()));
				
				int ir = (int)(255.99 * col.r());
				int ig = (int)(255.99 * col.g());
				int ib = (int)(255.99 * col.b());
				sb.append(ir + " " + ig + " " + ib + "\n");
			}
		}
		
       try {  
            Writer w = new FileWriter("Diffuse.ppm");  
            w.append(sb);  
            w.close();  
            long totalTime = System.nanoTime() - startTime;
            double seconds = (double)totalTime / 1000000000.0;
            System.out.println("Completed render in: " + seconds + " seconds.");  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
	}
	
	private static Vec3 random_in_unit_sphere() {
		Vec3 p;
		Random rand = new Random();

		do {
			Vec3 randVec = new Vec3(rand.nextFloat(), rand.nextFloat(), rand.nextFloat());
			Vec3 unitVec = new Vec3(1.0f, 1.0f, 1.0f);
			p = randVec.mul(2.0f).sub(unitVec);
		} while (p.squared_length() >= 1.0f);
		
		return p;
	}
	
	private static Vec3 color(ray r, hitable_list world) {
		hit_record rec = new hit_record();
		rec = world.hit(r,  0.001f, Float.MAX_VALUE, rec);
		if(rec.valid) {
			Vec3 target = rec.p.add(rec.normal).add(random_in_unit_sphere());
			
			// Change the -1 to 1 coordinates to 0 to 1
			// 0.5f * new Vec3(N.x() + 1f, N.y() + 1f, N.z() + 1f)
			return color(new ray(rec.p, target.sub(rec.p)), world).mul(0.5f);
		} else {
			Vec3 unit_direction = r.direction().normalize();
			float t = (float) (0.5f * (unit_direction.y() + 1.0));
			Vec3 white = new Vec3(1.0f, 1.0f, 1.0f);
			Vec3 blue = new Vec3(0.5f, 0.7f, 1.0f);
			return white.mul(1.0f - t).add(blue.mul(t));
		}
	}

}
