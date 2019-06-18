
#include "stats.h"

stats::stats(PNG & im){
  int height = im.height();
  int width = im.width();
   for(int i = 0; i < width; i++) {
    vector<double> col;
    for (int j =0; j<height; j++) {
      col.push_back(0);
    }
    sumSat.push_back(col);
    sumLum.push_back(col);
    sumHueX.push_back(col);
    sumHueY.push_back(col);
   }
   sumSat[0][0] = im.getPixel(0,0) ->s;
   sumLum[0][0] = im.getPixel(0,0) ->l;
   sumHueX[0][0] = cos(im.getPixel(0,0) ->h / 180 * PI);
   sumHueY[0][0] = sin(im.getPixel(0,0) ->h / 180 * PI);

   for (int i =1; i< width; i++){
     sumSat[i][0] = sumSat[i-1][0] + im.getPixel(i,0) ->s;
     sumLum[i][0] = sumLum[i-1][0] + im.getPixel(i,0) ->l;
     sumHueX[i][0] = sumHueX[i-1][0] + cos(im.getPixel(i,0) ->h / 180 * PI);
     sumHueY[i][0] = sumHueY[i-1][0] + sin(im.getPixel(i,0) ->h / 180 * PI);
   }
   for (int i= 1; i<height; ++i) {
     sumSat[0][i] = sumSat[0][i-1] + im.getPixel(0,i) ->s;
     sumLum[0][i] = sumLum[0][i-1] + im.getPixel(0,i) ->l;
     sumHueX[0][i] = sumHueX[0][i-1] + cos(im.getPixel(0,i) ->h / 180 * PI);
     sumHueY[0][i] = sumHueY[0][i-1] + sin(im.getPixel(0,i) ->h / 180 * PI);
   }
   for (int i=1; i <width; i++) {
     for (int j =1; j< height; j++) {
      sumSat[i][j]= sumSat[i-1][j] + sumSat[i][j-1] -sumSat[i-1][j-1] + im.getPixel(i,j)->s;
      sumLum[i][j]= sumLum[i-1][j] + sumLum[i][j-1] -sumLum[i-1][j-1] + im.getPixel(i,j)->l;
      sumHueX[i][j]= sumHueX[i-1][j] + sumHueX[i][j-1] -sumHueX[i-1][j-1] + cos(im.getPixel(i,j) ->h / 180 * PI);
      sumHueY[i][j]= sumHueY[i-1][j] + sumHueY[i][j-1] -sumHueY[i-1][j-1] + sin(im.getPixel(i,j) ->h / 180 * PI);
     }
   }


   for(int i = 0; i < width; i++) {
    vector<vector<int>> col;
    for (int j =0; j<height; j++) {
      vector<int> freq;
      for (int k=0; k<36; ++k) {
        freq.push_back(0);
      }
      col.push_back(freq);
    }
    hist.push_back(col);
}

int h = (int)(im.getPixel(0,0) -> h/10);
hist[0][0][h] = 1;
   for (int i =1; i< width; i++){
       for (int k=0; k<36; ++k) {
     hist[i][0][k] =  hist[i-1][0][k];
   }
   h = (int)(im.getPixel(i,0)->h/10);
   hist[i][0][h] ++;
}
   for (int i =1; i< height; i++){
       for (int k=0; k<36; ++k) {
     hist[0][i][k] =  hist[0][i-1][k];
   }
   h = (int)(im.getPixel(0,i)->h/10);
   hist[0][i][h] ++;
}
   for (int i =1; i< width; i++){
     for (int j =1; j<height; j++) {
       for (int k=0; k<36; ++k) {
     hist[i][j][k] =  hist[i-1][j][k] + hist[i][j-1][k] -hist[i-1][j-1][k];
   }
   h = (int)(im.getPixel(i,j)->h/10);
   hist[i][j][h] ++;
}

}

}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */
return (lr.second - ul.second +1)*(lr.first-ul.first +1);

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
      double totalHueX, totalHueY, totalSat, totalLum, avgHueX, avgHueY, avgSat, avgLum, avgHue;
      totalSat = sumSat[lr.first][lr.second];
      totalLum = sumLum[lr.first][lr.second];
      totalHueX = sumHueX[lr.first][lr.second];
      totalHueY = sumHueY[lr.first][lr.second];


      if (ul.first!=0 && ul.second!=0) {
        totalSat = sumSat[lr.first][lr.second]
                - sumSat[lr.first][ul.second-1]
                - sumSat[ul.first-1][lr.second]
                + sumSat[ul.first-1][ul.second-1];

        totalLum = sumLum[lr.first][lr.second]
                - sumLum[lr.first][ul.second-1]
                - sumLum[ul.first-1][lr.second]
                + sumLum[ul.first-1][ul.second-1];

        totalHueX = sumHueX[lr.first][lr.second]
                - sumHueX[lr.first][ul.second-1]
                - sumHueX[ul.first-1][lr.second]
                + sumHueX[ul.first-1][ul.second-1];
        totalHueY = sumHueY[lr.first][lr.second]
                - sumHueY[lr.first][ul.second-1]
                - sumHueY[ul.first-1][lr.second]
                + sumHueY[ul.first-1][ul.second-1];

      }
      else if (ul.first!=0) {
        totalSat -=  sumSat[ul.first-1][lr.second];
        totalLum -=  sumLum[ul.first-1][lr.second];
        totalHueX -= sumHueX[ul.first-1][lr.second];
        totalHueY -= sumHueY[ul.first-1][lr.second];
      }
      else if (ul.second!=0){
        totalSat -=  sumSat[lr.first][ul.second-1];
        totalLum -=  sumLum[lr.first][ul.second-1];
        totalHueX -= sumHueX[lr.first][ul.second-1];
        totalHueY -= sumHueY[lr.first][ul.second-1];
      }
      else {
        totalSat = sumSat[lr.first][lr.second];
        totalLum = sumLum[lr.first][lr.second];
        totalHueX = sumHueX[lr.first][lr.second];
        totalHueY = sumHueY[lr.first][lr.second];
      }



      avgHueX = totalHueX/rectArea(ul,lr);
      avgHueY = totalHueY/rectArea(ul,lr);
      avgSat = totalSat/rectArea(ul,lr);
      avgLum = totalLum/rectArea(ul,lr);
      avgHue = atan2(avgHueY,avgHueX)* 180/PI;
      if (avgHue <0) {
          avgHue += 360;
      }
    return HSLAPixel(avgHue,avgSat,avgLum);


}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

vector<int> dist(36);
 for (int i =0; i < 36; i++) {
   dist[i] = hist[lr.first][lr.second][i];
 }
 if (ul.first) {
   for (int i=0; i< 36; i++) {
     dist[i] -= hist[ul.first-1][lr.second][i];
   }
 }
 if (ul.second) {
   for (int i=0; i<36; i++){
     dist[i] -= hist[lr.first][ul.second-1][i];
   }
 if (ul.first) {

   for (int i=0; i<36; i++) {
     dist[i] += hist[ul.first-1][ul.second-1][i];
   }
  }
 }

return dist;

}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;


    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 )
            entropy += ((double) distn[i]/(double) area)
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

  vector<int> dist = buildHist(ul,lr);

return entropy(dist,rectArea(ul,lr));

}
