#include "gui.h"
#include "animable.h"

#include <SFML/Graphics/RenderWindow.hpp>

using namespace soul;

void GuiDebugLog::render(GameWindow& gw) {

    ImGui::SetNextWindowSize(ImVec2(400, 250), ImGuiCond_FirstUseEver);

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    if (!ImGui::Begin("DebugLog")) {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool bt_clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool bt_copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();

    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
        if (bt_clear)
            clear();
        if (bt_copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive()) {
        //     // In this example we don't use the clipper when Filter is enabled.
        //     // This is because we don't have random access to the result of our filter.
        //     // A real application processing logs with ten of thousands of entries may want to store the result of
        //     // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++) {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
            // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
            // within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
            // on your side is recommended. Using ImGuiListClipper requires
            // - A) random access into your data
            // - B) items all being the  same height,
            // both of which we can handle since we have an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display
            // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
            // it possible (and would be recommended if you want to search through tens of thousands of entries).
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step()) {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
        // Using a scrollbar or mouse-wheel will take away from the bottom edge.
        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::End();
}



void GuiAnimableStates::init() {
    tmp_pos_x = animable.getPosition().x;
    tmp_pos_y = animable.getPosition().y;
    tmp_jumpForce = animable.getTransform().initialVelocityY;
    tmp_gravityForce = animable.getTransform().gravity;
}

void GuiAnimableStates::render(GameWindow& gw) {
    // ImGui::SetNextWindowPos(ImVec2(game.window.getSize().x - 300, game.window.getSize() + 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);

    ImGui::Begin("Player Stats");
    
    ImGui::InputInt("PosX", &tmp_pos_x);
    ImGui::InputInt("PosY", &tmp_pos_y);
    ImGui::InputInt("Jump Force", &tmp_jumpForce);
    ImGui::InputInt("Gravity Force", &tmp_gravityForce);

    if (ImGui::Button("Update")) {        
        auto tr = animable.getTransform();
        tr.initialVelocityY = tmp_jumpForce;
        tr.gravity = tmp_gravityForce;

        animable.setPosition(tmp_pos_x, tmp_pos_y);
        animable.setTransform(tr);

        LoggerManager::getInstance().log("Player attributes change event: Pos({},{}) JumpForce {} Gravity {}", 
            animable.getPosition().x, animable.getPosition().y,
            animable.getTransform().initialVelocityY, animable.getTransform().gravity);
    }

    ImGui::Text("Position : %f, %f", animable.getPosition().x, animable.getPosition().y);

    auto scale = animable.getSprite().getScale();
    // auto velocity = stats.getVelocity();
    ImGui::Text("Sprite Scale x,y : %.0f, %.0f", scale.x, scale.y);

    // ImGui::Text("Velocity x,y : %d, %d", velocity.x, velocity.y);
    ImGui::Text("Jumping : %s", animable.input.is_jumping ? "Yes" : "No");
    ImGui::Text("Punching : %s", animable.input.punch ? "Yes" : "No");
    ImGui::Text("Shooting : %s", animable.input.is_shooting ? "Yes" : "No");
    
    ImGui::End();
}

void GuiSpriteTest::init(const std::string& spriteFilename) {

    _spritePosition.x = 975;
    _spritePosition.y = 10;

    _pos.x = 0;
    _pos.y = 0;
    _size.x = 45;
    _size.y = 80;
    _scale.x = 2.0f;
    _scale.y = 2.0f;

    _sprite->loadTexture(spriteFilename, _scale, true);

    auto initialSpritePosition = Vector2i(0, 0);
    auto initialSpriteSize = Vector2i(_size.x, _size.y);

    // Initial sprite to load
    _sprite->setTextureRect(
        initialSpritePosition.x, initialSpritePosition.y,
        initialSpriteSize.x, initialSpriteSize.y);
}

void GuiSpriteTest::render(GameWindow& gw) {

    _sprite->getSprite().setPosition(sf::Vector2f(_spritePosition.x, _spritePosition.y));

    ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Sprite Test");
        ImGui::InputInt("SpPosX", &_spritePosition.x);
        ImGui::InputInt("SpPosY", &_spritePosition.y);
        ImGui::InputInt("TexPosX", &_pos.x);
        ImGui::InputInt("TexPosY", &_pos.y);
        ImGui::InputInt("TexSizeX", &_size.x);
        ImGui::InputInt("TexSizeY", &_size.y);
        ImGui::InputFloat("TexScaleX", &_scale.x);
        ImGui::InputFloat("TexScaleY", &_scale.y);

        if (ImGui::Button("Update")) {        
            _sprite->setTextureRect(_pos.x, _pos.y, _size.x, _size.y);
            _sprite->setScale(_scale.x, _scale.y);
            LoggerManager::getInstance().log("Tex Position/Size update: ({},{}) ({},{})", _pos.x, _pos.y, _size.x, _size.y);
        }

        ImGui::Text("Sprite Position : %d, %d", _spritePosition.x, _spritePosition.y);
        ImGui::Text("Tex Position : %d, %d", _pos.x, _pos.y);
        ImGui::Text("Tex Size : %d, %d", _size.x, _size.y);
    ImGui::End();
}


LoggerGui::LoggerGui(const LOG_LEVEL level, shared_ptr<GuiDebugLog>& p_guiDebugLog)
    : ILogger(level), guiDebugLog(p_guiDebugLog) {

}

LoggerGui::~LoggerGui() {}

void LoggerGui::write(const LOG_LEVEL level, const std::string& s) {

    guiDebugLog->addLog("%s\n", s.c_str());
}
