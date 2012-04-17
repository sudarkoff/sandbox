
template <typename T>
void ptr_delete(const volatile T*&)
{
	return;
}

int main()
{
    const volatile CCardPropertiesLegacy* p = pCardProperties;    // works
    const volatile CCardPropertiesLegacy* &q = pCardProperties;   // fails

    ptr_delete (p); // works

	ptr_delete( pCardProperties );

	ptr_delete( pEepromList );
} 
