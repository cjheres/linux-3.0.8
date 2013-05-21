#ifndef	__GALOIS_PINMUX_HEADER__
#define	__GALOIS_PINMUX_HEADER__

#define	PINMUX_IOCTL_READ			0x5100
#define	PINMUX_IOCTL_WRITE			0x5101
#define	PINMUX_IOCTL_SETMOD			0x5102
#define	PINMUX_IOCTL_GETMOD			0x5103
#define	PINMUX_IOCTL_PRINTSTATUS	0x5104

#define	SOC_PINMUX					0
#define	SM_PINMUX					1

typedef	struct galois_pinmux_data{
	int	owner;				//	soc or sm pinmux
	int group;				//	group id
	int value;				//	group value
	char * requster;		//	who request the change
}galois_pinmux_data_t;

#endif
