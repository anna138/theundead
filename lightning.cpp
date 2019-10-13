#include <cmath>
#include <iostream>
#include <lightning.h>
void Draw(SpriteBatch,spritBatch, Color color) {
    Vector2 tangent =B-A;
    float rotation =(float)Math.Atan2(tangent.Y,tangent.X);

    const float ImageThickness=8;
    float thicknessScale = Thickness / ImageThickness;
    std::vector<int> capOrigin = Vector2(Art.HalfCircle.Width, Art.HalfCircle.Height/2f);
    std::vector<int> middleOrigin = Vector2(0, Art.LightningSegment.Height/2f);
    std::vector<int> middleScale = Vector2(tangent.Length(), thicknessScale/2f);

    spriteBatch.Draw(Art.LightingSegment, A, null, color, rotation, middleOrigin,middleScale.SpriteEffects.None,0f);

    spriteBatch.Draw(Art.HalfCircle, A, null, color, rotation,capOrigin, thicknessScale,SpriteEffects.None,0f);
    spriteBatch.Draw(Art.HalfCircle, B, null, color, rotation+MathHelper.Pi,capOrigin, thicknessScale,SpriteEffects.None,0f);
}


