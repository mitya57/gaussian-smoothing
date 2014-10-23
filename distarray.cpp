#include <algorithm>
#include <QtCore/qmath.h>
#include <QtCore/QList>
#include <QtCore/QTextStream>
#include "distarray.h"

bool getNeighbour(QPoint currentInd, QSize size, unsigned j, QPoint &neighbour) {
    if (j == 4) {
        return false;
    }
    QPoint diff(j % 3, j / 3);
    if (!(currentInd.x() || diff.x()) || !(currentInd.y() || diff.y())) {
        return false;
    }
    neighbour.setX(currentInd.x() + diff.x() - 1);
    neighbour.setY(currentInd.y() + diff.y() - 1);
    return (neighbour.x() < size.width()) && (neighbour.y() < size.height());
}

void fillDistanceArray(PixelArray const &array,
                       qreal            *distArray)
/* We expect that distArray points to a memory of the same
 * size as array.pixels. */
{
    QPoint neighbour;
    QList<unsigned> processList;
    qreal dist;
    unsigned num;
    unsigned elementsCount = array.size.width() * array.size.height();

    for (num = 0; num < elementsCount; ++num) {
        if (!array[num]) {
            distArray[num] = 0;
            processList.push_back(num);
        } else {
            distArray[num] = -1;
        }
    }

    while (!processList.empty()) {
        /* States:
         * - processed: distArray[array.num(index)] != -1 && not in the set
         * - toProcess: in the set
         * - null state: distArray[array.num(index)] == -1 && not in the set
         *
         * Procedure:
         * 1) currentInd := (one in the set with lowest diff)
         * 2) update distances for all neighbours that are not already
         *    processed, insert them to the set if not yet there,
         *    and move them if already there
         * 3) remove currentInd from the set
         */

#ifndef NO_DEBUG
        QTextStream(stdout) << "\rRemaining points to process: "
                            << processList.size();
#endif

        std::sort(processList.begin(), processList.end(),
                  [&distArray](unsigned num1, unsigned num2) -> bool {
            /* First, order by distance */
            if (!qFuzzyCompare(distArray[num1], distArray[num2])) {
                return distArray[num1] < distArray[num2];
            }
            /* If distances are equal, order by j */
            return num1 < num2;
        });

        unsigned currentNum = *processList.begin();
        QPoint currentInd = array.index2d(currentNum);

        for (unsigned char j = 0; j < 9; ++j) {
            QPoint diff(j % 3, j / 3);
            if (!getNeighbour(currentInd, array.size, j, neighbour)) {
                continue;
            }

            dist = distArray[currentNum] + qSqrt(
                qPow(diff.x() - 1, 2) +
                qPow(diff.y() - 1, 2)
            );
            num = array.num(neighbour);
            if (distArray[num] < -.5) {
                distArray[num] = dist;
                if (!processList.contains(num)) {
                    processList.push_back(num);
                }
            } else if (distArray[num] > dist) {
                distArray[num] = dist;
            }
        }
        processList.pop_front();
    }
#ifndef NO_DEBUG
    QTextStream(stdout) << "\rAll points processed successfully!" << endl;
#endif
}
