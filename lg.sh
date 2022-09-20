#TOKEN=$(http get https://amdocs.globalfitnesschallenge.com/v1/auth/login/me?utcOffset=120 'Authorization: Basic YWxleGFuZGVyZGFpbHkwMDFAZ21haWwuY29tOkhhbGwwVzNsdCE=' -v | grep -oP '(?<=Set-Cookie: jwtToken=)[^;]*')
http get https://amdocs.globalfitnesschallenge.com/v1/auth/login/me?utcOffset=120 'Authorization: Basic YWxleGFuZGVyZGFpbHkwMDFAZ21haWwuY29tOkhhbGwwVzNsdCE=' -v 
#echo $TOKEN

