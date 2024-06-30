import XCTest
@testable import MiniAudio
import Foundation

final class MiniAudioTests: XCTestCase {
    func testPlayback() throws {
        let resourceUrl = Bundle.module.url(
          forResource: "street",
          withExtension: "mp3"
        )
        let audioData = try Data(contentsOf: resourceUrl!)
        let player = MiniAudio.AudioPlayer()
        try player.initAudioPlaybackDevice(forPlay: audioData)
        try player.startAudioPlaying()
        sleep(UInt32(player.getDuration()))
        player.closeAudioPlaybackDevice()
    }

    func testCapture() throws {
        do {
            let capturer = AudioCapturer()
            try capturer.initAudioCaptureDevice(EncodingFormat.wav, AudioFormat.s16, 1, 16000)
            try capturer.startAudioCapturing()
            sleep(5)
            capturer.closeAudioCaptureDevice()

            let data = capturer.getData()
            let player = MiniAudio.AudioPlayer()
            try player.initAudioPlaybackDevice(forPlay: data)
            try player.startAudioPlaying()
            sleep(UInt32(player.getDuration()))
            player.closeAudioPlaybackDevice()
        } catch {
            XCTFail("Unexpected error: \(error)")
        }
    }
}
