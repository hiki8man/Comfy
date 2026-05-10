#include "TimelineRenderHelper.h"
#include "System/ComfyData.h"
#include "ImGui/Extensions/ImGuiExtensions.h"

namespace Comfy::Studio::Editor
{
	void TimelineRenderHelper::OnEditorSpritesLoaded(const Graphics::SprSet* sprSet)
	{
		editorSprites = sprSet;
		if (editorSprites == nullptr)
			return;

		auto findSprite = [this](std::string_view spriteName)
		{
			const auto found = FindIfOrNull(editorSprites->Sprites, [&](const auto& spr) { return spr.Name == spriteName; });
			return (found != nullptr && InBounds(found->TextureIndex, editorSprites->TexSet.Textures)) ? found : nullptr;
		};

		sprites.ButtonIcons[static_cast<size_t>(ButtonType::Triangle)] = findSprite("TIMELINE_TRIANGLE");
		sprites.ButtonIcons[static_cast<size_t>(ButtonType::Square)] = findSprite("TIMELINE_SQUARE");
		sprites.ButtonIcons[static_cast<size_t>(ButtonType::Cross)] = findSprite("TIMELINE_CROSS");
		sprites.ButtonIcons[static_cast<size_t>(ButtonType::Circle)] = findSprite("TIMELINE_CIRCLE");
		sprites.ButtonIcons[static_cast<size_t>(ButtonType::SlideL)] = findSprite("TIMELINE_SLIDE_L");
		sprites.ButtonIcons[static_cast<size_t>(ButtonType::SlideR)] = findSprite("TIMELINE_SLIDE_R");
		sprites.ButtonIcons[static_cast<size_t>(ButtonType::Star)] = findSprite("TIMELINE_STAR");

		sprites.ButtonIconsChance[static_cast<size_t>(ButtonType::Triangle)] = findSprite("TIMELINE_TRIANGLE_CH");
		sprites.ButtonIconsChance[static_cast<size_t>(ButtonType::Square)] = findSprite("TIMELINE_SQUARE_CH");
		sprites.ButtonIconsChance[static_cast<size_t>(ButtonType::Cross)] = findSprite("TIMELINE_CROSS_CH");
		sprites.ButtonIconsChance[static_cast<size_t>(ButtonType::Circle)] = findSprite("TIMELINE_CIRCLE_CH");
		sprites.ButtonIconsChance[static_cast<size_t>(ButtonType::SlideL)] = findSprite("TIMELINE_SLIDE_L_CH");
		sprites.ButtonIconsChance[static_cast<size_t>(ButtonType::SlideR)] = findSprite("TIMELINE_SLIDE_R_CH");
		sprites.ButtonIconsChance[static_cast<size_t>(ButtonType::Star)] = findSprite("TIMELINE_STAR_CH");

		sprites.ButtonIconsSync[static_cast<size_t>(ButtonType::Triangle)] = findSprite("TIMELINE_TRIANGLE_SYNC");
		sprites.ButtonIconsSync[static_cast<size_t>(ButtonType::Square)] = findSprite("TIMELINE_SQUARE_SYNC");
		sprites.ButtonIconsSync[static_cast<size_t>(ButtonType::Cross)] = findSprite("TIMELINE_CROSS_SYNC");
		sprites.ButtonIconsSync[static_cast<size_t>(ButtonType::Circle)] = findSprite("TIMELINE_CIRCLE_SYNC");
		sprites.ButtonIconsSync[static_cast<size_t>(ButtonType::SlideL)] = findSprite("TIMELINE_SLIDE_L_SYNC");
		sprites.ButtonIconsSync[static_cast<size_t>(ButtonType::SlideR)] = findSprite("TIMELINE_SLIDE_R_SYNC");
		sprites.ButtonIconsSync[static_cast<size_t>(ButtonType::Star)] = findSprite("TIMELINE_STAR");

		sprites.ButtonIconsFrag[static_cast<size_t>(ButtonType::SlideL)] = findSprite("TIMELINE_SLIDE_CHAIN_L");
		sprites.ButtonIconsFrag[static_cast<size_t>(ButtonType::SlideR)] = findSprite("TIMELINE_SLIDE_CHAIN_R");
		sprites.ButtonIconsFragSync[static_cast<size_t>(ButtonType::SlideL)] = findSprite("TIMELINE_SLIDE_CHAIN_L_SYNC");
		sprites.ButtonIconsFragSync[static_cast<size_t>(ButtonType::SlideR)] = findSprite("TIMELINE_SLIDE_CHAIN_R_SYNC");

		sprites.ButtonIconsDouble[static_cast<size_t>(ButtonType::Triangle)] = findSprite("TIMELINE_TRIANGLE_W");
		sprites.ButtonIconsDouble[static_cast<size_t>(ButtonType::Circle)] = findSprite("TIMELINE_CIRCLE_W");
		sprites.ButtonIconsDouble[static_cast<size_t>(ButtonType::Cross)] = findSprite("TIMELINE_CROSS_W");
		sprites.ButtonIconsDouble[static_cast<size_t>(ButtonType::Square)] = findSprite("TIMELINE_SQUARE_W");
		sprites.ButtonIconsDouble[static_cast<size_t>(ButtonType::Star)] = findSprite("TIMELINE_STAR_W");

		sprites.ButtonIconsLong[static_cast<size_t>(ButtonType::Triangle)] = findSprite("TIMELINE_TRIANGLE_LONG");
		sprites.ButtonIconsLong[static_cast<size_t>(ButtonType::Circle)] = findSprite("TIMELINE_CIRCLE_LONG");
		sprites.ButtonIconsLong[static_cast<size_t>(ButtonType::Cross)] = findSprite("TIMELINE_CROSS_LONG");
		sprites.ButtonIconsLong[static_cast<size_t>(ButtonType::Square)] = findSprite("TIMELINE_SQUARE_LONG");
		sprites.ButtonIconsLong[static_cast<size_t>(ButtonType::Star)] = nullptr;

		sprites.HoldText = findSprite("TIMELINE_HOLD_TEXT");
		sprites.HoldTextSync = findSprite("TIMELINE_HOLD_TEXT_SYNC");
		sprites.TargetLine = findSprite("TARGET_LINE");
		sprites.TargetLineYellow = findSprite("TARGET_LINE_YELLOW");

	}

	void TimelineRenderHelper::DrawButtonIcon(ImDrawList* drawList, const TimelineTarget& target, vec2 position, f32 scale, f32 transparency) const
	{
		if (target.Flags.SameTypeSyncCount > 1)
		{
			scale *= sameTypeSyncStackScale;
			position += (target.Flags.SameTypeSyncIndex == 0) ? +sameTypeSyncStackOffset : -sameTypeSyncStackOffset;
		}

		const auto radius = buttonIconWidth * scale;

		const auto topLeft = position - (radius * 0.5f);
		const auto bottomRight = position + (radius * 0.5f);

		// Let Target Chance looks better
		const auto chancetopLeft = topLeft - 2.0f;
		const auto chancebottomRight = bottomRight + 2.0f;

		const auto color = IM_COL32(0xFF, 0xFF, 0xFF, 0xFF * transparency);

		if (target.Flags.IsChance)
		{
			if (const auto chancebuttonSpr = GetChanceButtonSpriteForTarget(target); chancebuttonSpr != nullptr)
				Gui::AddSprite(drawList, *editorSprites, *chancebuttonSpr, chancetopLeft, chancebottomRight, color);
			else
				drawList->AddText(Gui::GetFont(), 12.0f, position + vec2(-18.0f, 0.0f), color, "CHANCE");
		}
		else 
		{
			if (const auto buttonSpr = GetButtonSpriteForTarget(target); buttonSpr != nullptr)

				Gui::AddSprite(drawList, *editorSprites, *buttonSpr, topLeft, bottomRight, color);
			else
				drawList->AddRect(position - vec2(radius * 0.25f), position + vec2(radius * 0.25f), color);
		}

		if (target.Flags.IsHold)
		{
			if (const auto holdTextSpr = GetHoldTextSprite(target.Flags.IsSync); holdTextSpr != nullptr)
				Gui::AddSprite(drawList, *editorSprites, *holdTextSpr, topLeft, bottomRight, color);
			else
				drawList->AddText(Gui::GetFont(), 12.0f, position + vec2(-12.0f, 0.0f), color, "HOLD");
		}

#if COMFY_DEBUG && 0 // DEBUG: DOUBLE FLAG TEST
		if (target.Flags.IsDouble)
			drawList->AddText(Gui::GetFont(), 12.0f, position + vec2(-12.0f, 0.0f), color, "DOUBLE");
#endif

#if COMFY_DEBUG && 0 // DEBUG: LONG FLAG TEST
		if (target.Flags.IsLong)
			drawList->AddText(Gui::GetFont(), 12.0f, position + vec2(0.0f, 6.0f), color, "LONG");
#endif

#if COMFY_DEBUG && 0 // DEBUG: CHAIN FLAG TEST
		if (target.Flags.IsChainStart)
			drawList->AddCircle(position, 14.0f, 0xFFFFFF00);
		if (target.Flags.IsChainEnd)
			drawList->AddCircle(position, 9.0f, 0xFFFF00FF);
#endif

#if COMFY_DEBUG && 0 // DEBUG: SYNC INDEX FLAG TEST
		char buffer[64];
		drawList->AddCircleFilled(position, 8.0f, IM_COL32_BLACK);
		drawList->AddText(position - vec2(3.0f, 9.0f), IM_COL32_WHITE, buffer, buffer + sprintf_s(buffer, "%d", target.Flags.IndexWithinSyncPair));
#endif
	}

	void TimelineRenderHelper::DrawTargetLine(ImDrawList* drawList, ButtonType type, vec2 startPos, vec2 endPos, f32 scale, f32 opacity) const
	{
		if (sprites.TargetLine == nullptr || sprites.TargetLineYellow == nullptr)
			return;

		const float width  = endPos.x - startPos.x;
		const float height = 18.0f * scale;

		const auto* spr = type == ButtonType::Star ? sprites.TargetLineYellow : sprites.TargetLine;
		const auto& tex = editorSprites->TexSet.Textures[spr->TextureIndex];

		const vec2 uv0 = vec2(spr->TexelRegion.x, 1.0f - spr->TexelRegion.w);
		const vec2 uv1 = vec2(spr->TexelRegion.x, 1.0f - spr->TexelRegion.y);
		const vec2 uv2 = vec2(spr->TexelRegion.z, 1.0f - spr->TexelRegion.y);
		const vec2 uv3 = vec2(spr->TexelRegion.z, 1.0f - spr->TexelRegion.w);

		ImU32 color = 0xFFFFFFFF;
		switch (type)
		{
		case ButtonType::Triangle:
			color = IM_COL32(0, 230, 128, static_cast<i32>(opacity * 255));
			break;
		case ButtonType::Square:
			color = IM_COL32(240, 147, 226, static_cast<i32>(opacity * 255));
			break;
		case ButtonType::Cross:
			color = IM_COL32(63, 152, 255, static_cast<i32>(opacity * 255));
			break;
		case ButtonType::Circle:
			color = IM_COL32(255, 84, 92, static_cast<i32>(opacity * 255));
		}

		// NOTE: This is probably *veeeeery* unefficient because of the many draw calls
		//       for a single line, but using repeat UVs would require editing the sprite
		//       and textures are a lot more stuff for it to properly repeat and like... eh.
		//       I guess this is good enough.
		float drawnWidth = 0.0f;
		while (drawnWidth < width)
		{
			const vec2 topLeft = vec2(startPos.x + drawnWidth, startPos.y - height / 2.0f);
			const vec2 topRight = vec2(startPos.x + drawnWidth + 2.0f, endPos.y - height / 2.0f);
			const vec2 bottomLeft = vec2(startPos.x + drawnWidth, startPos.y + height / 2.0f);
			const vec2 bottomRight = vec2(startPos.x + drawnWidth + 2.0f, endPos.y + height / 2.0f);

			drawList->AddImageQuad(*tex, bottomLeft, topLeft, topRight, bottomRight, uv0, uv1, uv2, uv3, color);
			drawnWidth += 2.0f;
		}
	}

	const Graphics::Spr* TimelineRenderHelper::GetButtonSpriteForTarget(const TimelineTarget& target) const
	{
		const auto typeIndex = static_cast<u8>(target.Type);
		const bool isSync = target.Flags.IsSync;
		bool isFrag = (target.Flags.IsChain && !target.Flags.IsChainStart);
		bool isDouble = target.Flags.IsDouble;
		bool isLong = target.Flags.IsLong;
		bool isChance = target.Flags.IsChance;

		// NOTE: This does not match the correct behavior as used in the render window but should avoid confusion between single fragment chains and normal slides
		if (target.Flags.IsChainStart && target.Flags.IsChainEnd)
			isFrag = true;

		const auto& typesArray =
			isLong ? sprites.ButtonIconsLong :
			isDouble ? sprites.ButtonIconsDouble :
			isChance ? sprites.ButtonIconsChance :
			isFrag ? isSync ? sprites.ButtonIconsFragSync : sprites.ButtonIconsFrag :
			isSync ? sprites.ButtonIconsSync : sprites.ButtonIcons;

		return IndexOr(typeIndex, typesArray, nullptr);
	}

	const Graphics::Spr* TimelineRenderHelper::GetHoldTextSprite(const bool isSync) const
	{
		return isSync ? sprites.HoldTextSync : sprites.HoldText;
	}

	const Graphics::Spr* TimelineRenderHelper::GetChanceButtonSpriteForTarget(const TimelineTarget& target) const
	{
		const auto typeIndex = static_cast<u8>(target.Type);
		const auto& typesArray = sprites.ButtonIconsChance;

		return IndexOr(typeIndex, typesArray, nullptr);
	}
}
