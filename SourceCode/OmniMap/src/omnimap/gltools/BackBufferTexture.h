#ifndef __BACK_BUFFER_TEXTURE_H__
#define __BACK_BUFFER_TEXTURE_H__

class BackBufferTexture {
public:
	BackBufferTexture();
	~BackBufferTexture();
	void CreateTexture(int _width, int _height);
	void CopyFB2Texture();
	void BindTexture();
	void UnBindTexture();
	void setViewport();

private:
	unsigned int textureID;
	int width;
	int height;
	unsigned int targetType;
	bool ispoweroftwo(int num);
};

#endif
