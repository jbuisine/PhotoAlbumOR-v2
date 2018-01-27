library(ggplot2)

this.dir <- dirname(parent.frame(2)$ofile)
setwd(this.dir)

readFile <- function(filename){
  return(read.csv(header = FALSE, file = filename, sep = " "))
}

dfStats <- readFile("output.txt")
dfPF_FRRMAB <- readFile("front_pa_frrmab.txt")
dfPF_FRRMAB_DK <- readFile("front_pa_frrmab_dk.txt")

# Erase sol index info
dfPF_FRRMAB <- dfPF_FRRMAB[, 1:3]
dfPF_FRRMAB_DK <- dfPF_FRRMAB_DK[, 1:3]

# head of dfStats
names(dfStats) <- c("ID", "Fitness", "Obj1", "Obj2", "DIR", "FROM", "BEST", "OP")
names(dfPF_FRRMAB) <- c("Fitness", "Obj1", "Obj2")
names(dfPF_FRRMAB_DK) <- c("Fitness", "Obj1", "Obj2")

drawOpDistrubution <- function(){
  
  repOp <- prop.table(table(dfStats$OP[dfStats$OP >= 0]))
  barplot(repOp, 
          main="Operators' distribution", 
          xlab="Index of op",
          ylim = c(0,1))
}

drawPF <- function(){
  
  ggplot() + 
    geom_point(data=dfPF_FRRMAB, aes(x=Obj1, y=Obj2), color='blue') + geom_smooth(method = 'loess') 
}

