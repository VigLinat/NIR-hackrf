SdrException::SdrException(hackrf_error result)
{
	m_result = result;
}

std::string SdrException::What() const
{
	return "hackrf failed: " + std::string(hackrf_error_name(m_result));
}