#include "HRFTransceiver.h"
#include "../SDRException/SDRException.h"
HRFTransceiver::HRFTransceiver()
{
	m_params = nullptr;
	m_fileToSend = std::ifstream();
}

HRFTransceiver::HRFTransceiver(const std::shared_ptr<HRFUtil::HRFParams> params, const std::wstring filename)
{
	m_params = params;

	if (!m_fileToSend.is_open()) 
	{
		m_fileToSend = std::ifstream();
		m_fileToSend.open(filename, std::ios::binary);
	}
}

void HRFTransceiver::Transfer(hackrf_device* device)
{
	int result = 0;
	result = hackrf_set_txvga_gain(device, m_params->txvga_gain);
	result |= hackrf_start_tx(device, tx_callback, NULL);

	if (result != HACKRF_SUCCESS)
	{
		std::cerr << "Transfer failed: ";
		throw SDRException((hackrf_error)result);
	}
}

void HRFTransceiver::Receive()
{

}

int HRFTransceiver::tx_callback(hackrf_transfer* transfer)
{
	if (m_fileToSend.is_open() && m_fileToSend.good())
	{
		size_t buffer_length = transfer->valid_length;
		size_t bits_to_read = buffer_length / 8;
		char* read_buffer = (char*)malloc(bits_to_read);
		m_fileToSend.read(read_buffer, bits_to_read);

		size_t bits_read = m_fileToSend.gcount();

		make_psk2_buffer(transfer->buffer, read_buffer, bits_read);

		if (bits_read != bits_to_read)
		{
			if (m_params->repeat)
			{
				size_t extra_bits = bits_to_read - bits_read;
				std::cerr << "Input file end reached. Rewind to beginning" << std::endl;
				m_fileToSend.seekg(0);
				m_fileToSend.read(read_buffer + bits_read, extra_bits);
				make_psk2_buffer(transfer->buffer + bits_read * 8, read_buffer + bits_read, extra_bits);
				free(read_buffer);
				return 0;
			}
			else
			{
				free(read_buffer);
				return -1;
			}
		}
		else
		{
			free(read_buffer);
			return 0;
		}
	}
	else
	{
		return -1;
	}
}

void HRFTransceiver::make_psk2_buffer(uint8_t* buffer, char* data, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 7; j >= 0; j--)
		{
			uint8_t modulation_symbol = (data[i] & (1 << i)) != 0 ? 127 : -128;
			buffer[i * 8 + j] = modulation_symbol;
		}
	}
}