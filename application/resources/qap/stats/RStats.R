this.dir <- dirname(parent.frame(2)$ofile)
setwd(this.dir)

readFile <- function(filename){
  return(read.csv(header = FALSE, file = filename, sep = " "))
}

dfStats <- readFile("output.txt")
dfPF <- readFile("front_pa.txt")

# Erase sol index info
dfPF <- dfPF[, 1:3]

# head of dfStats
names(dfStats) <- c("ID", "Fitness", "Obj1", "Obj2", "DIR", "FROM", "BEST", "OP")
names(dfPF) <- c("Fitness", "Obj1", "Obj2")

drawOpDistrubution <- function(){
  
  repOp <- prop.table(table(dfStats$OP[dfStats$OP >= 0]))
  barplot(repOp, 
          main="Operators' distribution", 
          xlab="Index of op",
          ylim = c(0,1))
}

drawPF <- function(){
  
  plot(dfPF$Obj1 ~ dfPF$Obj2,
       main = "Pareto front",
       xlab = "Objective 2",
       ylab = "Objective 1")
}

p1<-ggplot(dfStats, aes(x=Obj1, y=Obj2, colour="Black")) + 
  geom_smooth(se=F)
p1

