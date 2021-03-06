#ifndef TUPLE3_H 
#define TUPLE3_H 
 

 
 
#include "Tuple2.h" 

 
template <class T> 
class Tuple3 
{ 
  public: 
    Tuple3()	//构造函数 
    { 
      x = 0; 
      y = 0; 
      z = 0; 
    } 
 
    Tuple3(T nx, T ny, T nz)	//构造函数 
    { 
      x = nx; 
      y = ny; 
      z = nz; 
    } 
 
    Tuple3(T xyz)//构造函数 
    { 
      x =  
      y =  
      z = xyz; 
    } 
 
    Tuple3(Tuple3* tuple){//构造函数 
      x = tuple->x; 
      y = tuple->y; 
      z = tuple->z; 
    } 
 
    Tuple3(const Tuple3 &source){//构造函数 
      x = source.x; 
      y = source.y; 
      z = source.z; 
    } 
 
    Tuple3(const T* source){//构造函数 
      x = source[0]; 
      y = source[1]; 
      z = source[2]; 
    } 
 
    Tuple3(T* source){//构造函数 
      x = source[0]; 
      y = source[1]; 
      z = source[2]; 
    } 
 
    Tuple3(const Tuple2<T> &source){//构造函数 
      x = source.x; 
      y = source.y; 
      z = 1.0; 
    } 
 
    Tuple3(const Tuple2<T> &source, const T Z){//构造函数 
      x = source.x; 
      y = source.y; 
      z = Z; 
    } 
 
    inline Tuple3 &operator = (const Tuple3 &right){//运算符重载 
      x = right.x; 
      y = right.y; 
      z = right.z; 
      return *this; 
    } 
  
    inline Tuple3 &operator = (const T* source){ 
      x = source[0]; 
      y = source[1]; 
      z = source[2]; 
      return *this; 
    } 
 
    inline Tuple3 &operator =( T* source){ 
      x = source[0]; 
      y = source[1]; 
      z = source[2]; 
      return *this; 
    } 
 
    inline Tuple3 operator + (const Tuple3 &right){ 
      return Tuple3(right.x + x, right.y + y, right.z + z); 
    } 
  
    inline Tuple3 operator - (const Tuple3 &right){ 
      return Tuple3(-right.x + x, -right.y + y, -right.z + z); 
    } 
  
    inline Tuple3 operator * (const T scale){ 
      return Tuple3(x*scale, y*scale, z*scale); 
    } 
  
    inline Tuple3  operator / (const T scale){ 
      if(scale) return Tuple3f(x/scale, y/scale, z/scale); 
      return Tuple3(0, 0, 0); 
    } 
 
    inline void operator += (const Tuple3 &right){ 
      x+=right.x; 
      y+=right.y; 
      z+=right.z; 
    } 
 
    inline void operator += (T xyz){ 
      x += xyz; 
      y += xyz; 
      z += xyz; 
    } 
 
    inline void operator -= (const Tuple3 &right) 
    { 
      x-=right.x; 
      y-=right.y; 
      z-=right.z; 
    } 
 
    inline void operator -= (T xyz) 
    { 
      x -= xyz; 
      y -= xyz; 
      z -= xyz; 
    } 
 
    inline void operator *= (T scale)  
    { 
      x*=scale; 
      y*=scale; 
      z*=scale; 
    } 
 
    inline void operator /= (T scale) 
    { 
      if(!scale)  
	      return; 
      x/=scale; 
      y/=scale; 
      z/=scale; 
    } 
 
    bool operator == (const Tuple3 &right) 
    { 
      return (x == right.x && 
              y == right.y && 
              z == right.z); 
    } 
 
    bool operator != (const Tuple3 &right) 
    { 
      return !(x == right.x && 
               y == right.y && 
               z == right.z); 
    } 
 
    inline operator const T*() const { return &x; } 
    inline operator T*() { return &x; }    
 
	  inline const T  operator[](int i) const { return ((T*)&x)[i]; } 
	  inline       T &operator[](int i)       { return ((T*)&x)[i]; } 
 
    inline void set(T nx, T ny, T nz) 
    { 
      x = nx; 
      y = ny; 
      z = nz; 
    } 
 
    inline void set(const Tuple2<T> & vec, const T Z) 
    { 
      x = vec.x; 
      y = vec.y; 
      z = Z;    
    } 
 
    inline void set(T xyz) 
    { 
      x =    
      y =   
      z = xyz; 
    } 
 
    inline void set(const Tuple3& t) 
    { 
      x = t.x; 
      y = t.y; 
      z = t.z; 
    } 
 
    inline Tuple3 &normalize()//单位化向量 
    { 
      T length  = getLength(); 
 
      if(!length){ 
        set(0,0,0); 
        return *this; 
      } 
      x/=length; 
      y/=length; 
      z/=length; 
      return *this; 
    } 
 
    inline T getLengthSquared(){ return  x*x + y*y + z*z; } 
    inline T getLength()       { return sqrtf(x*x + y*y + z*z); }//向量长度 
 
    inline T getDotProduct(const Tuple3 &t) //点积 
    { 
      return x*t.x + y*t.y + z*t.z; 
    } 
 
    inline Tuple3 operator ^(const Tuple3 &t) //叉积 
    { 
      return Tuple3(y   * t.z - z   * t.y, 
                    t.x * z   - t.z * x, 
                    x   * t.y - y   * t.x); 
    } 
 
    inline Tuple3 &operator ^=(const Tuple3 &t)//叉积 
    { 
       set(y   * t.z - z   * t.y, 
           t.x * z   - t.z * x, 
           x   * t.y - y   * t.x); 
       return *this; 
    } 
 
    inline Tuple3 &crossProduct(const Tuple3 &t1, const Tuple3 &t2)//叉积 
    { 
      set(t1.y * t2.z - t1.z * t2.y, 
          t2.x * t1.z - t2.z * t1.x, 
          t1.x * t2.y - t1.y * t2.x); 
      return *this; 
    } 
 
    inline T getDistance( Tuple3& v2)	//两向量之间的距离 
    { 
      return sqrtf((v2.x - x) * (v2.x - x) + 
                   (v2.y - y) * (v2.y - y) + 
                   (v2.z - z) * (v2.z - z)); 
    } 
 
    inline T getAngle(Tuple3& v2)	//两向量之间的角度 
    {              
               
      T angle = acos(getDotProduct(v2) / (getLength() * v2.getLength())); 
      if(_isnan(angle)) 
        return 0; 
      return angle ; 
    } 
 
    inline void clamp(T min, T max) 
    { 
      x = x > max ? max : x  min ? min  : x; 
      y = y > max ? max : y  min ? min  : y; 
      z = z > max ? max : z  min ? min  : z; 
    } 
 
    friend ostream & operator < ( ostream & out, const Tuple3 & right){ 
      return out < "Tuple(" < right.x < ", " < right.y < ", " < right.z <<")"; 
    } 
 
    friend istream & operator >> (istream & in, Tuple3 & right){ 
      return in >> right.x >> right.y >> right.z; 
    } 
    T x, y, z; 
}; 
 
typedef Tuple3<int>           Tuple3i; 
typedef Tuple3<float>         Tuple3f; 
typedef Tuple3<float>		  tVector; 
typedef Tuple3<float>		  CVector3D; 
typedef Tuple3<float>		  Point3D; 
typedef Tuple3<float>		  Color3f; 
typedef Tuple3<double>        Tuple3d; 
typedef Tuple3<char>          Tuple3b; 
typedef Tuple3<unsigned char> Tuple3ub; 
#endif 