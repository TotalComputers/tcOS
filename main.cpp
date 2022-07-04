#include "client/protocol/protocol.h"
#include "client/ByteBuffer.h"
#include "client/client.h"
#include "client/pipeline/pipeline.h"
#include <iostream>

class TestIO : public IOInterface {
public:
    void init() override {
        std::cout << "IO::init {id=" << id <<", name=" << name << "}" << std::endl;
        for(int y = 0; y < buffer.height; y++) {
            for(int x = 0; x < buffer.width; x++) {
                int idx = y * buffer.width + x;
                color_argb_t color;
                color.a = 255;
                color.r = (uint8_t)((double(x) / buffer.width) * 255);
                color.g = (uint8_t)((double(y) / buffer.height) * 255);
                color.b = 0;
                buffer.data[idx] = color;
            }
        }
    }

    void destroy() override {
        std::cout << "IO::destroy (" << id << ")" << std::endl;
    }

    image_raw8_t provide_frame() override {
        std::cout << "IO::provide_frame" << std::endl;
        return buffer.as_c_image().as_raw_image().as_raw_image();
    }

    void set_frame(image_t frame) override {
        std::cout << "IO::set_frame {width=" << frame.width << ", height=" << frame.height << "}" << std::endl;
        buffer = frame;
    }
    
    void handle_touch(int x, int y, bool type, bool admin) override {
        std::cout << "IO::handle_touch {x=" << x << ", y=" << y << ", is_right=" << type << ", admin=" << admin << "}" << std::endl;
    }

public:
    image_t buffer;

};

class TestIOFactory : public IOFactory {
public:
    std::shared_ptr<IOInterface> create() override {
        return std::make_shared<TestIO>();
    }

};

int main() {
    std::string ip;
    std::string token;
    int port;

    std::cout << "Enter IP (Without port): ";
    std::cin >> ip;
    std::cout << "Enter port: ";
    std::cin >> port;
    std::cout << "Enter token: ";
    std::cin >> token;

    protocol_registerPackets();

    Pipeline* pipeline = new Pipeline();

    pipeline->addLast("packet_decoder", new PacketDecoder());
    pipeline->addLast("packet_encoder", new PacketEncoder());
    pipeline->addLast("packet_handler", new PacketHandler(token, new TestIOFactory()));

    tcp_set_pipeline(pipeline);    

    tcp_connect(ip.c_str(), port);
}