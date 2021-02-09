class SdrException
{
public:
	SdrException() = default;
	 
	SdrException(const hackrf_error result);
	
	std::string What() const;

private:
	hackrf_error m_result;
};