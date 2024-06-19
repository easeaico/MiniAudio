import MiniAudioC
import Foundation

public enum MiniAudioErrors: Error {
    case initAudioDeviceFailed
    case playingAudioFailed
    case capturingAudioFailed
}

public enum EncodingFormat {
    case unknown, wav, flac, mp3,vorbis

    func maType() -> MiniAudioC.ma_encoding_format {
        switch self  {
        case .unknown:
            return MiniAudioC.ma_encoding_format_unknown
        case .wav:
            return MiniAudioC.ma_encoding_format_wav
        case .flac:
            return MiniAudioC.ma_encoding_format_flac
        case .mp3:
            return MiniAudioC.ma_encoding_format_mp3
        case .vorbis:
            return MiniAudioC.ma_encoding_format_vorbis
        }
    }
}

public enum AudioFormat {
    case unknown, u8, s16, s24, s32, f32, count

    func maType() -> MiniAudioC.ma_format {
        switch self {
        case .unknown:
            return MiniAudioC.ma_format_unknown
        case .u8:
            return MiniAudioC.ma_format_u8
        case .s16:
            return MiniAudioC.ma_format_s16
        case .s24:
            return MiniAudioC.ma_format_s24
        case .s32:
            return MiniAudioC.ma_format_s32
        case .f32:
            return MiniAudioC.ma_format_f32
        case .count:
            return MiniAudioC.ma_format_count
        }
    }
}

public class AudioCapturer {
    var data: MiniAudioC.AudioData

    public init() {
        self.data = MiniAudioC.AudioData(buffer: nil, size: 0, offset: 0)
    }

    deinit {
        if (self.data.buffer != nil) {
            free(self.data.buffer)
        }
    }

    public func initAudioCaptureDevice(_ encodingFormat: EncodingFormat,
                                  _ format: AudioFormat, 
                                  _ channels: UInt32,
                                  _ sampleRate: UInt32) throws  {
        let (ef, af) = (encodingFormat.maType(), format.maType())
        let result = MiniAudioC.initAudioCaptureDevice(ef, af, channels, sampleRate, &self.data)
        if result != 0 {
            throw MiniAudioErrors.initAudioDeviceFailed
        }
    }

    public func startAudioCapturing() throws {
        let result = MiniAudioC.startAudioCapturing()
        if result != 0 {
            throw MiniAudioErrors.capturingAudioFailed
        }
    }

    public func closeAudioCaptureDevice() {
        MiniAudioC.closeAudioCaptureDevice()
    }
    
    public func getData() -> Data {
        return Data(bytes: self.data.buffer, count: self.data.size)
    }
}

public class AudioPlayer {
    public func initAudioPlaybackDevice(forPlay data: Data) throws {
        try data.withUnsafeBytes { rawBufferPointer in
            let pointer = rawBufferPointer.bindMemory(to: UInt8.self).baseAddress!
            let result = MiniAudioC.initAudioPlackbackDevice(pointer, data.count)
            if result != 0 {
                throw MiniAudioErrors.initAudioDeviceFailed
            }
        }
    }
    
    public func startAudioPlaying() throws {
        let result = MiniAudioC.startAudioPlaying()
        if result != 0 {
            throw MiniAudioErrors.playingAudioFailed
        }
    }
    
    public func closeAudioPlaybackDevice() {
        MiniAudioC.closeAudioPlaybackDevice()
    }
}
