import XCTest
@testable import MiniAudio
import Foundation

final class MiniAudioTests: XCTestCase {
    func testPlayback() throws {
        let url1 = Bundle.module.url(forResource: "street1", withExtension: "mp3")!
        let url2 = Bundle.module.url(forResource: "street2", withExtension: "mp3")!
        let url3 = Bundle.module.url(forResource: "street3", withExtension: "mp3")!
        
        let data1 = try Data(contentsOf: url1)
        let data2 = try Data(contentsOf: url2)
        let data3 = try Data(contentsOf: url3)
        
        let player = MiniAudio.AudioPlayer()
        
        try player.initDeviceOrUpdate(for: data1)
        try player.startAudioPlaying()
        sleep(player.getDuration())
        try player.stopAudioPlaying()
        
        try player.initDeviceOrUpdate(for: data2)
        try player.startAudioPlaying()
        sleep(player.getDuration())
        try player.stopAudioPlaying()
        
        try player.initDeviceOrUpdate(for: data3)
        try player.startAudioPlaying()
        sleep(player.getDuration())
        try player.stopAudioPlaying()
        
        player.closePlaybackDevice()
    }

    func testCapture() throws {
        do {
            let capturer = AudioCapturer()
            try capturer.initCaptureDevice(EncodingFormat.wav, AudioFormat.s16, 1, 16000)
            try capturer.startAudioCapturing()
            sleep(5)
            capturer.closeCaptureDevice()

            let data = capturer.getData()
            let player = MiniAudio.AudioPlayer()
            try player.initDeviceOrUpdate(for: data)
            try player.startAudioPlaying()
            sleep(player.getDuration())
            try player.stopAudioPlaying()
            player.closePlaybackDevice()
        } catch {
            XCTFail("Unexpected error: \(error)")
        }
    }
}
