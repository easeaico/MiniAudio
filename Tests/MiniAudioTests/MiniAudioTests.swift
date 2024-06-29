import XCTest
@testable import MiniAudio
import Foundation

final class MiniAudioTests: XCTestCase {
    func testPlayback() throws {
        let resourceUrl = Bundle.module.url(
          forResource: "dune",
          withExtension: "wav"
        )
        let audioData = try Data(contentsOf: resourceUrl!)
        let player = MiniAudio.AudioPlayer()
        try player.initAudioPlaybackDevice(forPlay: audioData)
        try player.startAudioPlaying()
        Thread.sleep(forTimeInterval: Double(player.getDuration()))
        player.closeAudioPlaybackDevice()
    }

    func testCapture() throws {
        do {
            let capturer = AudioCapturer()
            try capturer.initAudioCaptureDevice(EncodingFormat.wav, AudioFormat.s16, 1, 16000)
            try capturer.startAudioCapturing()
            Thread.sleep(forTimeInterval: 20)
            capturer.closeAudioCaptureDevice()

            let data = capturer.getData()
            let player = MiniAudio.AudioPlayer()
            try player.initAudioPlaybackDevice(forPlay: data)
            try player.startAudioPlaying()
            Thread.sleep(forTimeInterval: Double(player.getDuration()))
            player.closeAudioPlaybackDevice()
        } catch {
            XCTFail("Unexpected error: \(error)")
        }
    }
}
