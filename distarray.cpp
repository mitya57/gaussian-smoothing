#include <set>
#include <QtCore/qmath.h>
#include "distarray.h"

struct SortableIndex2D: QPoint {
    /* Some additional fields that are needed for sorting */
    SortableIndex2D (QPoint const &_index2d,
                     PixelArray const &_array,
                     qreal const *_distArray):
        QPoint(_index2d), array(_array), distArray(_distArray) {}

    PixelArray const &array;
    qreal const *distArray;
};

struct SortableIndexCompare2D {
    bool operator() (SortableIndex2D const &ind1,
                     SortableIndex2D const &ind2) const
    {
        unsigned num1 = ind1.array.num(ind1);
        unsigned num2 = ind2.array.num(ind2);
        /* First, order by distance */
        if (!qFuzzyCompare(ind1.distArray[num1], ind2.distArray[num2])) {
            return ind1.distArray[num1] < ind2.distArray[num2];
        }
        /* If distances are equal, order by j */
        return num1 < num2;
    }
};

void fillDistanceArray(PixelArray const &array,
                       qreal            *distArray)
/* We expect that distArray points to a memory of the same
 * size as array.pixels. */
{
    QPoint neighbour;
    std::set<SortableIndex2D, SortableIndexCompare2D> processSet;
    qreal dist;
    unsigned num;
    unsigned elementsCount = array.size.width() * array.size.height();

    for (num = 0; num < elementsCount; ++num) {
        QPoint currentInd = array.index2d(num);
        if (!array[num]) {
            distArray[num] = 0;
            processSet.insert(SortableIndex2D(
                              currentInd, array, distArray));
        } else {
            distArray[num] = -1;
        }
    }

    while (!processSet.empty()) {
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

        SortableIndex2D const currentInd = *processSet.begin();
        if (processSet.find(currentInd) == processSet.end()) {
          return;
        }

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

            dist = distArray[array.num(currentInd)] + qSqrt(
                qPow(array.pixelSize.width() * (diff.x() - 1), 2) +
                qPow(array.pixelSize.height() * (diff.y() - 1), 2)
            );
            num = array.num(neighbour);
            if (distArray[num] < -.5) {
                distArray[num] = dist;
                processSet.insert(SortableIndex2D(neighbour, array, distArray));
            } else if (distArray[num] > dist) {
                distArray[num] = dist;
            }
        }
        processSet.erase(currentInd);
    }
}
