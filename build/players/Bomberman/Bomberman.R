input <- commandArgs(TRUE)
history <- do.call(rbind,strsplit(scan(textConnection(input[2]),""),","))
health <- as.integer(history[,1])
last_shooter <- which(grepl("S",history[-1,ncol(history)]))
last_prepare <- which(history[1,]=="P")
if(!length(last_prepare)) last_prepare <- -1
last_throw <- which(grepl("T",history[1,]))
if(!length(last_throw)) last_throw <- 0
most_healthy <- which.max(health[-1])
dead <- which(health[-1]<=0)
inoffensive <- c(last_shooter,dead)
danger <- which(!(1:3)%in%inoffensive)
alive <- which(!(1:3)%in%dead)
if(health[1]>3 & last_throw > last_prepare) out <- "P"
if(last_throw < last_prepare) out <- ifelse(length(last_shooter),paste("T",last_shooter[1],sep=""),paste("T",most_healthy[1],sep=""))
if(health[1]<=3 & last_throw > last_prepare){
    if(length(danger)){
        out <- paste("D",sample(danger,1),sep="")
    }else{
        out <- paste("S",sample(alive,1),sep="")
    }
}
cat(out)
