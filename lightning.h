#include <ctime>
public class Line {
    public: Vector2 A;
            Vector2 B;
            float Thickness;

            Line() {  }

            Line(Vector2 a, Vector2 b, float thickness=1) {
                A=a;
                B=b;
                Thickness=thickness;
            }

};
