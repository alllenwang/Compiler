int fact(int n)
{
	if(n == 1)
		return n;
	else
		return (n * fact(n-1));

}
int main()
{
	int m,result;
	int array[4];
	array[1] = 0;
	m = read();
	if(m > 1)
		result = fact(m);
	else
		result = 1;
	write(result);
	return 0;
}
