declare -a ACTIONS=($(cat ./export.xml | tr -d ' '))

LOG=()

for ACTION in "${ACTIONS[@]:0:13}"; do 
#for ACTION in "${ACTIONS[@]}"; do 
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
  echo "$DATE - ${LOG[$KEY]}"
done

