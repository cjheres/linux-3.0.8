#define MAX_CHANNELS 8
typedef enum {
	MULTI_PATH = 0,
	LoRo_PATH = 1,
	SPDIF_PATH = 2,
	HDMI_PATH = 3,
	MAX_OUTPUT_AUDIO = 4,
} AUDIO_PATH;

typedef enum {
	MAIN_AUDIO = 0,
	SECOND_AUDIO = 1,
	EFFECT_AUDIO = 2,
	EXT_IN_AUDIO = 10,
	BOX_EFFECT_AUDIO = 11,
	MAX_INTERACTIVE_AUDIOS = 8,
	MAX_INPUT_AUDIO = 12,
	AUDIO_INPUT_IRRELEVANT,
	NOT_DEFINED_AUDIO,
} AUDIO_CHANNEL;

INT32 pri_audio_chanId[4] =
    { avioDhubChMap_ag_MA0_R, avioDhubChMap_ag_MA1_R, avioDhubChMap_ag_MA2_R,
      avioDhubChMap_ag_MA3_R };

typedef struct aout_dma_info_t {
	UINT32 addr0;
	UINT32 size0;
	UINT32 addr1;
	UINT32 size1;
} AOUT_DMA_INFO;

typedef struct aout_path_cmd_fifo_t {
	AOUT_DMA_INFO aout_dma_info[4][8];
	UINT32 update_pcm[8];
	UINT32 takeout_size[8];
	UINT32 size;
	UINT32 wr_offset;
	UINT32 rd_offset;
	UINT32 kernel_rd_offset;
	UINT32 zero_buffer;
	UINT32 zero_buffer_size;
	UINT32 fifo_underflow;
} AOUT_PATH_CMD_FIFO;

typedef struct aip_dma_cmd_t {
	UINT32 addr0;
	UINT32 size0;
	UINT32 addr1;
	UINT32 size1;
} AIP_DMA_CMD;

typedef struct aip_cmd_fifo_t {
	AIP_DMA_CMD aip_dma_cmd[4][8];
	UINT32 update_pcm[8];
	UINT32 takein_size[8];
	UINT32 size;
	UINT32 wr_offset;
	UINT32 rd_offset;
	UINT32 kernel_rd_offset;
	UINT32 prev_fifo_overflow_cnt;
	/* used by kernel */
	UINT32 kernel_pre_rd_offset;
	UINT32 overflow_buffer;
	UINT32 overflow_buffer_size;
	UINT32 fifo_overflow;
	UINT32 fifo_overflow_cnt;
} AIP_DMA_CMD_FIFO;

typedef struct {
	UINT *unaCmdBuf;
	UINT *cmd_buffer_base;
	UINT max_cmd_size;
	UINT cmd_len;
	UINT cmd_part1_len;
	UINT cmd_buffer_hw_base;
	UINT cmd_pattern;

	// cmd addr history
	UINT dma_cmd_in_offset[MAX_INPUT_AUDIO][MAX_CHANNELS];		// DMA in cmds ptr
	UINT dma_cmd_out0_offset[MAX_OUTPUT_AUDIO][MAX_CHANNELS];	// DMA out cmds ptr
	UINT dma_cmd_out1_offset[MAX_OUTPUT_AUDIO][MAX_CHANNELS];	// DMA out cmds ptr
	UINT dma_spdif_data_cmd_offset;		// spdif compressed data DMA in cmds ptr
	UINT dma_spdif_header_cmd_offset;	// spdif packing header DMA in cmds ptr
	UINT dma_hdmi_raw_data_cmd_offset;	// HDMI by-pass mode DMA in cmds ptr

	UINT dma_delay_cmd_in0_offset[MAX_OUTPUT_AUDIO][MAX_CHANNELS];
	UINT dma_delay_cmd_in1_offset[MAX_OUTPUT_AUDIO][MAX_CHANNELS];
	UINT dma_delay_cmd_out0_offset[MAX_OUTPUT_AUDIO][MAX_CHANNELS];
	UINT dma_delay_cmd_out1_offset[MAX_OUTPUT_AUDIO][MAX_CHANNELS];

	UINT dma_spdif_in_ddr_addr;	// spdif compressed data DMA in data ptr
	UINT dma_hdmi_raw_in_ddr_addr;	// hdmi compressed data DMA in data ptr
} APP_CMD_BUFFER;

typedef struct {
	UINT m_block_samps;
	UINT m_fs;
	UINT m_special_flags;
} HWAPP_DATA_DS;

typedef struct {
	HWAPP_DATA_DS m_stream_in_data[MAX_INPUT_AUDIO];
	HWAPP_DATA_DS m_path_out_data[MAX_OUTPUT_AUDIO];
} HWAPP_EVENT_CTX;

typedef struct {
	APP_CMD_BUFFER coef_cmd[8];
	APP_CMD_BUFFER data_cmd[8];
	HWAPP_EVENT_CTX m_tHwAppEventCtx[8];
	UINT size;
	UINT wr_offset;
	UINT rd_offset;
	UINT kernel_rd_offset;
/************** used by Kernel *****************/
	UINT kernel_idle;
} HWAPP_CMD_FIFO;

