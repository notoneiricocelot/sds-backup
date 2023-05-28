#pragma once

namespace SDS
{

	enum class SDSMenuType : int
	{
		kClassMenu,
		kClassCreationMenu,
		kBonfireLevelupMenu,
		kIconbasedInventoryMenu
	};

	enum class SDSFocus : int
	{
		kCombat = 1,
		kMagick = 2,
		kStealth = 3
	};

	enum class SDSAttribute : int
	{
		kStrength = 1,
		kIntelligence = 2,
		kWillpower = 3,
		kAgility = 4,
		kSpeed = 5,
		kEndurance = 6,
		kPersonality = 7,
		kLuck = 8,
		kFaith = 9
	};

	enum class InventoryDefine : int
	{
		kNone = 0,
		kArmor = 1,
		kWeapon = 2,
		kMisc = 3,
		kBook = 4,
		kFood = 5,
		kPotion = 6,
		kSpell = 7,
		kIngredient = 8,
		kKey = 9,
		kShout = 10,
		kActiveEffect = 11,
		kSoulGem = 12,
		kSpellDefault = 13,
		kList = 14,
		kCraftEnchanting = 15,
		kHousePart = 16
	};

	/*
	|  6   |  7  |   8   |   9   |   10  |   11  |   12  |  13  |  14  |   15  |  16  |   17   |  18 |  19  |  20  |  21 |  22  |  23  |
	|  1h  |  2h | marks | block | smith | heavy | light | pick | lock | sneak | alch | speech | alt | conj | dest | ill | rest | ench |
	*/

	class Specialization
	{
		public:
			Specialization(std::string i, std::string n, std::string d, SDSFocus f, SDSAttribute a1, SDSAttribute a2)
			{
				_ID = i;
				Name = n;
				Description = d;
				focus = f;
				firstAttribute = a1;
				secondAttribute = a2;
			}

			std::string_view GetID() const { return _ID; };
			std::string_view GetName() const { return Name; };
			std::string_view GetDescription() const { return Description; };

			SDSFocus GetFocus() const { return focus; }

			SDSAttribute GetFirstAttribute() const { return firstAttribute; }

			SDSAttribute GetSecondAttribute() const { return secondAttribute; }

			int* GetSkills() { return skills; }

			void SetSkillFocus(int avID, int pos)
			{
				skills[pos] = avID;
			}

			static inline std::vector<std::unique_ptr<Specialization>> Specializations;

		protected:
			std::string _ID;
		private:

			std::string Name;
			std::string Description;
			SDSFocus focus;
			SDSAttribute firstAttribute;
			SDSAttribute secondAttribute;
			int skills[6];
	};

	struct ScaleformHelper
	{
		public:
			static void FillPlayerObject(RE::GFxValue* playerObject);
			static void FillBottomBarObjectDefault(RE::GFxValue* bottombarObject);

		private:
			using AV = RE::ActorValue;
	};

}
