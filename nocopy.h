#ifndef TAK_NOCOPY_H__
#define TAK_NOCOPY_H__

#define NOCOPY(ClassName) private: ClassName(const ClassName&); ClassName& operator=(const ClassName&)

#endif // TAK_NOCOPY_H__
