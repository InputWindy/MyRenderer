#pragma once
#include <headers.h>
namespace math {
	class Line3
	{
	public:
		//��
		vec3	m_Orig;
		//����
		vec3	m_Dir;  
	public:
		Line3() {
			m_Orig = m_Dir = vec3(0.0f);
		};
		Line3(const vec3 Orig,const  vec3 Dir) {
			Set(Orig, Dir);
		};
		~Line3() {

		};
		void Set(const vec3 Orig,const  vec3 Dir) {
			m_Orig = Orig;
			m_Dir =normalize( Dir);
		};
		//����normalize
		void SetDir( vec3 Dir) {
			m_Dir =normalize( Dir);
		};
		void SetOrig( vec3 Orig) {
			m_Orig = Orig;
		};

	};

}
