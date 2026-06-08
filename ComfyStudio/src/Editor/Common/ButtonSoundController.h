#pragma once
#include "Types.h"
#include "Time/TimeSpan.h"
#include "SoundEffectManager.h"
#include <optional>

namespace Comfy::Studio::Editor
{
	enum class ButtonSoundType : u8
	{
		Button,
		Slide,
		ChainSlideFirst,
		ChainSlideSub,
		ChainSlideSuccess,
		ChainSlideFailure,
		SlideTouch,
		NormalW,
		Star,
		StarW,
		LongFirst,
		LongSub,
		LongEnd,
		Chance,
		Count,
	};

	enum class ChainSoundSlot : u8
	{
		Left,
		Right,
		Count,
	};

	class ButtonSoundController : NonCopyable
	{
	public:
		static constexpr size_t ButtonVoicePoolSize = 24;
		static constexpr size_t SliderTouchVoicePoolSize = 24;
		static constexpr size_t PerSlotChainVoicePoolSize = 12;
		// 长条音线程池尺寸
		static constexpr size_t PreSlotLongVoicePoolSize = 24;

	public:
		ButtonSoundController(SoundEffectManager& soundEffectManager);
		~ButtonSoundController();

	public:
		void SetIDs(u32 buttonID, u32 slideID, u32 chainSlideID, u32 sliderTouchID);

		
		void PlayButtonSound(TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});
		void PlaySlideSound(TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});
		void PlayStarSound(TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});
		void PlayNormalWSound(TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});

		// 长条音播放接口
		void PlayLongSoundStart(TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});
		void PlayLongSoundEnd(TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});

		void PlayChanceSound(TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});

		void PlayChainSoundStart(ChainSoundSlot slot, TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});
		void PlayChainSoundSuccess(ChainSoundSlot slot, TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});
		void PlayChainSoundFailure(ChainSoundSlot slot, TimeSpan startTime = TimeSpan::Zero(), std::optional<TimeSpan> externalClock = {});
		void FadeOutLastChainSound(ChainSoundSlot slot, TimeSpan startTime = TimeSpan::Zero());
		void FadeOutLastLongSound(TimeSpan startTime = TimeSpan::Zero());

		void PlaySliderTouch(i32 sliderTouchIndex, f32 baseVolume = 1.0f);

		void PauseAllChainSounds();
		void PauseAllLongSounds();
		void PauseAllNegativeVoices();

		f32 GetMasterVolume() const;
		void SetMasterVolume(f32 value);

	private:
		void InitializeVoicePools();
		void UnloadVoicePools();

		void PlayButtonSoundType(ButtonSoundType type, ChainSoundSlot slot, TimeSpan startTime, std::optional<TimeSpan> externalClock);

		Audio::SourceHandle GetSource(ButtonSoundType type, i32 sliderTouchIndex = 0) const;

	private:
		SoundEffectManager& soundEffectManager;

		f32 masterVolume = 1.0f;

		struct ButtonSoundIDs
		{
			u32 Button, Slide, ChainSlide, SliderTouch;
		} buttonIDs = {};

		struct SoundTypeTimeData
		{
			TimeSpan This, Last, SinceLast;

			void Update(std::optional<TimeSpan> externalClock);
			f32 GetVolumeFactor() const;
		};
		std::array<SoundTypeTimeData, EnumCount<ButtonSoundType>()> soundTimings = {};

		const TimeSpan chainFadeOutDuration = TimeSpan::FromMilliseconds(200.0);

		size_t buttonPoolRingIndex = 0, sliderTouchPoolRingIndex = 0 ;
		// 添加长条音线程池初始index
		size_t longStartPoolRingIndex = 0, longEndPoolRingIndex = 0, preSlotLongPoolRingIndex = 0;

		std::array<size_t, EnumCount<ChainSoundSlot>()> chainStartPoolRingIndices = {};
		std::array<size_t, EnumCount<ChainSoundSlot>()> chainEndPoolRingIndices = {};

		std::array<Audio::Voice, ButtonVoicePoolSize> buttonVoicePool;
		std::array<Audio::Voice, SliderTouchVoicePoolSize> sliderTouchVoicePool;
		std::array<std::array<Audio::Voice, PerSlotChainVoicePoolSize>, EnumCount<ChainSoundSlot>()> chainStartVoicePools, chainEndVoicePools;
		std::array<Audio::Voice, EnumCount<ChainSoundSlot>()> perSlotChainSubVoices;

		// 定义长条音线程池
		std::array<Audio::Voice, PreSlotLongVoicePoolSize> longStartVoicePool, longEndVoicePool;
		// 长条处于按住状态
		Audio::Voice preSlotLongVoices;
	};
}
