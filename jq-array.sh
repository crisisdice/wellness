# Read log
declare -a KEYS=($(echo $ACTIONS | jq -r 'keys' | tr -d '[]," '))

# Loop over log, loop over events in log
# for DATE in "${KEYS[@]}"; do 
#   declare -a VALUES=($(echo $ACTIONS | jq ".\"$DATE\"" | tr -d '[]," '))
#   for VALUE in "${VALUES[@]}"; do
#     # Format body
#     BODY="
#     {
#       \"timestamp\": \"$DATE\",
#       \"eventDescriptor\": {
#         \"name\": \"WALKING\"
#       },
#       \"value\": $VALUE,
#       \"validicEventId\": \"110867-1663586566603\",
#       \"customDescription\": null,
#       \"utcOffset\": 120
#     }
#     "
#     echo $BODY
#   done
# done

