
public class AABB {
    Vec3 _min;
    Vec3 _max;

    AABB(Vec3 a, Vec3 b){
        _min = a;
        _max = b;
    }
    
    AABB(){
    	_min = new Vec3(Float.MIN_VALUE, Float.MIN_VALUE, Float.MIN_VALUE);
    	_max = new Vec3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
    }

    public boolean hit(ray r, float tmin, float tmax){
        float t0 = ffmin((_min.x() - r.origin().x()) / r.direction().x(),
                (_max.x() - r.origin().x()) / r.direction().x());
        float t1 = ffmax((_min.x() - r.origin().x()) / r.direction().x(),
                (_max.x() - r.origin().x()) /r.direction().x());
        tmin = ffmax(t0, tmin);
        tmax = ffmin(t1, tmax);
        if(tmax <= tmin)
            return false;

        t0 = ffmin((_min.y() - r.origin().y()) / r.direction().y(),
                (_max.y() - r.origin().y()) / r.direction().y());
        t1 = ffmax((_min.y() - r.origin().y()) / r.direction().y(),
                (_max.y() - r.origin().y()) /r.direction().y());
        tmin = ffmax(t0, tmin);
        tmax = ffmin(t1, tmax);
        if(tmax <= tmin)
            return false;

        t0 = ffmin((_min.z() - r.origin().z()) / r.direction().z(),
                (_max.z() - r.origin().z()) / r.direction().z());
        t1 = ffmax((_min.z() - r.origin().z()) / r.direction().z(),
                (_max.z() - r.origin().z()) /r.direction().z());
        tmin = ffmax(t0, tmin);
        tmax = ffmin(t1, tmax);
        if(tmax <= tmin)
            return false;

        return true;
    }

    public boolean pixar_hit(ray r, float tmin, float tmax){
        float invD = 1.0f / r.direction().x();
        float t0 = (_min.x() - r.origin().x()) * invD;
        float t1 = (_max.x() - r.origin().x()) * invD;
        // if invD is negative swap t0 and t1
        if(invD < 0.0f){
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if(tmax <= tmin)
            return false;

        invD = 1.0f / r.direction().y();
        t0 = (_min.x() - r.origin().y()) * invD;
        t1 = (_max.x() - r.origin().y()) * invD;
        // if invD is negative swap t0 and t1
        if(invD < 0.0f){
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if(tmax <= tmin)
            return false;

        invD = 1.0f / r.direction().z();
        t0 = (_min.x() - r.origin().z()) * invD;
        t1 = (_max.x() - r.origin().z()) * invD;
        // if invD is negative swap t0 and t1
        if(invD < 0.0f){
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if(tmax <= tmin)
            return false;

        return true;
    }

    private float ffmin(float a, float b){
        return a < b ? a : b;
    }

    private float ffmax(float a, float b){
        return a > b ? a : b;
    }
}
