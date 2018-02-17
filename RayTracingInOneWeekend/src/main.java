import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.util.Random;

public class main {

	public static void main(String[] args) {
		System.out.println("Starting render, please wait...");
		long startTime = System.nanoTime();
		
		StringBuilder sb = new StringBuilder();
		int nx = 500;
		int ny = 400;
		int samples = 20;
		
		// Create all hitable spheres
		Random rand = new Random();
		hitable[] list = new hitable[5];
		list[0] = new sphere(new Vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(new Vec3(0.1f, 0.2f, 0.5f)));
		list[1] = new sphere(new Vec3(0.0f, -100.5f, -1.0f), 100, new lambertian(new Vec3(0.8f, 0.8f, 0.0f)));
		list[2] = new sphere(new Vec3(1f, 0f, -1f), 0.5f, new metal(new Vec3(0.8f, 0.6f, 0.2f), .0f));
		list[3] = new sphere(new Vec3(-1f, 0f, -1f), 0.5f, new dielectric(1.5f));
		list[4] = new sphere(new Vec3(-1f, 0f, -1f), -0.45f, new dielectric(1.5f));

		hitable_list world = new hitable_list(list, list.length);
		//world = random_scene();
		
		// Camera setup
		Vec3 lookfrom = new Vec3(13f, 2f, 3f);
		Vec3 lookat = new Vec3(0f, 0f, 0f);
		Vec3 vup = new Vec3(0f, 1f, 0f);
		float dist_to_focus = 10f;
		float aperture = 0.1f;
		
		camera cam = new camera(lookfrom, lookat, vup, 20f, (float)(nx)/(float)(ny), aperture, dist_to_focus, 0f, .5f);
		
		sb.append("P3\n" + nx + " " + ny + "\n255\n");
	
		for(int j = ny-1; j >=0; j--) {
			// Show percent done
			if((ny-1-j) % ((int)((ny-1)/100)) == 0) {
				System.out.println((ny-1-j) / ((int)((ny)/100)) + "% done." );
			}
			
			for(int i = 0; i < nx; i++) {
				Vec3 col = Vec3.zero();
				for(int s = 0; s < samples; s++) {
					float u = (float)(i + rand.nextFloat())/(float)(nx);
					float v = (float)(j + rand.nextFloat())/(float)(ny);
					
					ray r = cam.get_ray(u, v);
					Vec3 p = r.point_at_parameter(2.0f);
					// col += color(r, world)
					col = col.add(color(r, world, 0));
				}
				// col /= ns
				col = col.div(samples);
				
				// A basic gamma2 correction approximation. Just SQRT because they're 50% reflectors
				col = new Vec3((float)Math.sqrt(col.r()), (float)Math.sqrt(col.g()), (float)Math.sqrt(col.b()));
				
				int ir = (int)(255.99 * col.r());
				int ig = (int)(255.99 * col.g());
				int ib = (int)(255.99 * col.b());
				sb.append(ir + " " + ig + " " + ib + "\n");
			}
		}
		
		try {  
            Writer w = new FileWriter("BVHTest.ppm");
            w.append(sb);  
            w.close();  
            long totalTime = System.nanoTime() - startTime;
            double seconds = (double)totalTime / 1000000000.0;
            System.out.println("Completed render in: " + seconds + " seconds.");  
        } catch (IOException e) {  
            e.printStackTrace();  
        }  
	}
	
	public static Vec3 random_in_unit_sphere() {
		Vec3 p;
		Random rand = new Random();

		do {
			Vec3 randVec = new Vec3(rand.nextFloat(), rand.nextFloat(), rand.nextFloat());
			Vec3 unitVec = new Vec3(1.0f, 1.0f, 1.0f);
			p = randVec.mul(2.0f).sub(unitVec);
		} while (p.squared_length() >= 1.0f);
		
		return p;
	}
	
	private static Vec3 color(ray r, hitable_list world, int depth) {
		hit_record rec = world.hit(r,  0.001f, Float.MAX_VALUE, new hit_record());
		if(rec.valid) {
			ray scattered = rec.mat.scatter(r,  rec);
			if(depth < 50 && scattered.valid) {
				return rec.mat.attenuation.mul(color(scattered, world, depth+1));
			} else {
				return Vec3.zero();
			}
		} 
		// Draw background
		else {
			Vec3 unit_direction = r.direction().normalize();
			float t = (float) (0.5f * (unit_direction.y() + 1.0));
			Vec3 white = new Vec3(1.0f, 1.0f, 1.0f);
			Vec3 blue = new Vec3(0.5f, 0.7f, 1.0f);
			return white.mul(1.0f - t).add(blue.mul(t));
		}
	}
	
	private static hitable_list random_scene() {
		Random rand = new Random();
		
		int n = 500;
		hitable[] list = new hitable[n + 1];
		list[0] = new sphere(new Vec3(0f, -1000f, 0f), 1000f, new lambertian(new Vec3(0.5f, 0.5f, 0.5f)));
		int i = 1;
		for(int a = -11; a < 11; a++) {
			for(int b = -11; b < 11; b++) {
				float choose_mat = rand.nextFloat();
				Vec3 center = new Vec3(a + 0.9f * rand.nextFloat(), .2f, b + 0.9f * rand.nextFloat());
				if((center.sub(new Vec3(4f, .2f, 0f)).length() > 0.9f)){
					if(choose_mat < 0.2f) {	// Motion Blur Diffuse
						list[i++] = new moving_sphere(center, center.add(new Vec3(0f, rand.nextFloat() * 0.5f, .2f)), 0f, .5f, center.y(), 
								new lambertian(new Vec3(rand.nextFloat() * rand.nextFloat(), rand.nextFloat() * rand.nextFloat(), rand.nextFloat() * rand.nextFloat())));
					}
					else if(choose_mat < 0.5f) {	// Diffuse
						list[i++] = new sphere(center, center.y(), new lambertian(new Vec3(rand.nextFloat() * rand.nextFloat(), rand.nextFloat() * rand.nextFloat(), rand.nextFloat() * rand.nextFloat())));
					} else if(choose_mat < .92f) {	// Metal
						list[i++] = new sphere(center, center.y(), new metal(
								new Vec3(0.5f * (1 + rand.nextFloat()), 0.5f * (1 + rand.nextFloat()), 0.5f * (1 + rand.nextFloat())), rand.nextFloat()));
					} else { // glass
						list[i++] = new sphere(center, center.y(), new dielectric(1.5f));
					}
				}
			}
		}
		
		list[i++] = new sphere(new Vec3(0f, 1f, 0f), 1.0f, new dielectric(1.5f));
		list[i++] = new sphere(new Vec3(-4f, 1f, 0f), 1.0f, new lambertian(new Vec3(.4f, .2f, .1f)));
		list[i++] = new sphere(new Vec3(4f, 1f, 0f), 1.0f, new metal(new Vec3(.7f, .6f, .5f)));
		
		return new hitable_list(list, i);
	}

}
