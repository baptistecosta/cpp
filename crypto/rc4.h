#ifndef __ENCRYPTION_RC4__
#define __ENCRYPTION_RC4__

#define		ARRAY_LENGTH	256

//!
struct	RC4
{
	unsigned char	s[ARRAY_LENGTH];
	int				rc4_i, rc4_j;

	RC4();

	void			KSA(unsigned char* key, int key_len);
	char			PRGA();

	void			PrintArray();
	void			Swap(unsigned char* a, unsigned char* b);

};

#endif	//__ENCRYPTION_RC4__