# Server Authorization
TOKEN=''
COOKIE=''
XSRF=''
URL='https://amdocs.globalfitnesschallenge.com'
ENDPOINT='/v1/users/110867/events'
AGENT='Mozilla/5.0 (Windows NT 10.0; Win64; x64 AppleWebKit/537.36 )KHTML, like Gecko)( Chrome/105.0.0.0 Safari/537.36)'

declare -a ACTIONS=($(./a.out))

for BODY in "${ACTIONS[@]}"; do
  echo $BODY | http post $URL$ENDPOINT "Authorization: Bearer $TOKEN" "Cookie: $COOKIE" "Origin: $URL" "User-Agent: $AGENT" "X-XSRF-Token: $XSRF" -v
done

