#include <algorithm>
#include <QtCore/qmath.h>
#include <QtCore/QList>
#include "distarray.h"

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
            QPoint diff(j % 3,   j / 3);
            if (
                j == 4 || /* equal to currentInd */
                !(currentInd.x() || diff.x()) ||
                !(currentInd.y() || diff.y())
            ) {
                continue;
            }
            neighbour.setX(currentInd.x() + diff.x() - 1);
            neighbour.setY(currentInd.y() + diff.y() - 1);
            if (
                neighbour.x() >= array.size.width() ||
                neighbour.y() >= array.size.height()
            ) {
                continue;
            }

            dist = distArray[currentNum] + qSqrt(
                qPow(array.pixelSize.width() * (diff.x() - 1), 2) +
                qPow(array.pixelSize.height() * (diff.y() - 1), 2)
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
}
