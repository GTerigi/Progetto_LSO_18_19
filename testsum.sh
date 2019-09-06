#!/bin/bash

filename=testout.log

if [ ! -f $filename ]; then
    echo "$filename not exist"
    exit 1
fi

corr="correttamente"

#contatori per la Connessione
ConTot=0
ConOK=0

#contatori per la Store
StoreTot=0
StoreOK=0

#contatori per la Retrive
RetriveTot=0
RetriveOK=0

#contatori per la Delete
DeleteTot=0
DeleteOK=0

#contatore per le Disconnessioni
DiscTot=0
DiscOK=0

exec 3<$filename

while IFS=" " read -u 3 line; do
    read -r -a elem <<< "$line"
    case ${elem[1]} in 
    CONNESSIONE)
    ConTot=$((ConTot + 1))
    if [ "${elem[3]}" == "$corr" ]
    then
        ConOK=$((ConOK + 1))
    fi
    ;;
    OS_STORE)
    StoreTot=$((StoreTot + 1))
    if [ "${elem[3]}" == "$corr" ]
    then
        StoreOK=$((StoreOK + 1))
    fi
    ;;
    OS_RETRIVE)
    RetriveTot=$((RetriveTot + 1))
    if [ "${elem[3]}" == "$corr" ]
    then
        RetriveOK=$((RetriveOK + 1))
    fi
    ;;
    OS_DELETE)
    DeleteTot=$((DeleteTot + 1))
    if [ "${elem[3]}" == "$corr" ]
    then
        DeleteOK=$((DeleteOK + 1))
    fi
    ;;
    DISCONNESSIONE)
    DiscTot=$((DiscTot + 1))
    if [ "${elem[3]}" == "$corr" ]
    then
        DiscOK=$((DiscOK + 1))
    fi
    ;;
    esac
done

printf "\n\n\t\tValutazioni finali Progetto LSO 2018/2019\n"
printf "\nConnessioni totali :%d\n\tSuccessi:%d\n\tFalliti:%d\n" $ConTot $ConOK $((ConTot - ConOK)) 
printf "\nStore totali :%d\n\tSuccessi:%d\n\tFalliti:%d\n" $StoreTot $StoreOK $((StoreTot - StoreOK)) 
printf "\nRetrive totali :%d\n\tSuccessi:%d\n\tFalliti:%d\n" $RetriveTot $RetriveOK $((RetriveTot - RetriveOK)) 
printf "\nDelete totali :%d\n\tSuccessi:%d\n\tFalliti:%d\n" $DeleteTot $DeleteOK $((DeleteTot - DeleteOK)) 
printf "\nDisconnessioni totali :%d\n\tSuccessi:%d\n\tFalliti:%d\n" $DiscTot $DiscOK $((DiscTot - DiscOK)) 
