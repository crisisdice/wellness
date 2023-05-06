# https://www.howtogeek.com/782514/how-to-use-set-and-pipefail-in-bash-scripts-on-linux/
CUTOFF="2023-04-22" # inclusive 

PW_HASH=$(cat .password)
DEBUG="--offline"
SEARCH="grep" #macos ggrep

URL='https://amdocs.globalfitnesschallenge.com/v1'
POST_ENDPOINT='/users/110867/events'
AUTH_ENDPOINT='/auth/login/me?utcOffset=120'
AGENT='Mozilla/5.0 (Windows NT 10.0; Win64; x64 AppleWebKit/537.36 )KHTML, like Gecko)( Chrome/105.0.0.0 Safari/537.36)'

HEADERS=$(http GET $URL$AUTH_ENDPOINT "Authorization: Basic $PW_HASH" -v)

TOKEN=$(echo $HEADERS | $SEARCH -oP '(?<=Set-Cookie: jwtToken=)[^;]*')
XSRF=$(echo $HEADERS | $SEARCH -oP '(?<=Set-Cookie: XSRF-TOKEN=)[^;]*')
COOKIE="jwtToken=$TOKEN"

IFS=$'\n'
declare -a ACTIONS=($(wellness-xml-parser $CUTOFF))

for BODY in "${ACTIONS[@]}"; do
  echo $BODY | http $DEBUG POST $URL$POST_ENDPOINT "Authorization: Bearer $TOKEN" \
    "Cookie: $COOKIE" "Origin: $URL" "User-Agent: $AGENT" "X-XSRF-Token: $XSRF" -v
done

