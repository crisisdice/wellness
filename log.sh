# Server Authorization
TOKEN=''
COOKIE=''
XSRF=''
URL='https://amdocs.globalfitnesschallenge.com'
ENDPOINT='/v1/users/110867/events'
AGENT='Mozilla/5.0 (Windows NT 10.0; Win64; x64 AppleWebKit/537.36 )KHTML, like Gecko)( Chrome/105.0.0.0 Safari/537.36)'

# read export.xml from phone
declare -a ACTIONS=($(cat ./export.xml | tr -d ' '))

LOG=()

# increment dictionary with duration of activities
#for ACTION in "${ACTIONS[@]:0:13}"; do 
for ACTION in "${ACTIONS[@]}"; do 
  START=$(echo $ACTION | grep -oP '(?<=startDate=")[^+]*+')
  END=$(echo $ACTION | grep -oP '(?<=endDate=")[^+]*+')

  DIFF=$(dateutils.ddiff -i '%Y-%m-%d%H:%M:%S' -f '%Hh%Mm%Ss' $START $END)
  DATE=$(echo $START | dateutils.strptime -i '%Y-%m-%d%H:%M:%S' -f '%Y%m%d')

  CURRENT="${LOG[$DATE]}"

  if [ -z $CURRENT ]; then CURRENT='00:00:00'; fi

  NEW=$(dateutils.dadd -i '%H:%M:%S' $CURRENT $DIFF -f '%H:%M:%S')

  LOG["$DATE"]=$NEW

done

for KEY in "${!LOG[@]}"; do
  DATE=$(echo $KEY | dateutils.strptime -i '%Y%m%d' -f '%Y-%m-%dT12:00:00')
  VALUE="${LOG[$KEY]}"
  HOURS=$(echo $VALUE | dateutils.strptime -i '%H:%M:%S' -f '%H')
  MINUTES=$(($(echo $VALUE | dateutils.strptime -i '%H:%M:%S' -f '%M') + ($HOURS * 60)))
  # Format body
  TS=$(date +%s%N | cut -b1-13)
  ID="110867-$TS"

  BODY="
  {
    \"timestamp\": \"$DATE\",
    \"eventDescriptor\": {
      \"name\": \"WALKING\"
    },
    \"value\": $MINUTES,
    \"validicEventId\": \"$ID\",
    \"customDescription\": null,
    \"utcOffset\": 120
  }
  "
  echo $BODY | http post $URL$ENDPOINT "Authorization: Bearer $TOKEN" "Cookie: $COOKIE" "Origin: $URL" "User-Agent: $AGENT" "X-XSRF-Token: $XSRF" -v
done

