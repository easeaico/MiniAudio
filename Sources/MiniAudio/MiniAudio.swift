import MiniAudioC
import Foundation

public enum MiniAudioErrors: Error {
    case initDeviceFailed
    case decodeFormatFailed
    case playingAudioFailed
    case encodeFormatFailed
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
    private var ctx: MiniAudioC.CaptureContext
    private var data: AudioData

    public init() {
        self.ctx = MiniAudioC.CaptureContext(device: MiniAudioC.ma_device(), encoder: MiniAudioC.ma_encoder())
        self.data = MiniAudioC.AudioData(buffer: nil, size: 0, offset: 0)
    }

    deinit {
        if (self.data.buffer != nil) {
            free(self.data.buffer)
        }
    }

    public func initCaptureDevice(_ encodingFormat: EncodingFormat,
                                  _ format: AudioFormat, 
                                  _ channels: UInt32,
                                  _ sampleRate: UInt32) throws  {
        let (ef, af) = (encodingFormat.maType(), format.maType())
        let result = MiniAudioC.initCaptureDevice(&self.ctx, ef, af, channels, sampleRate, &self.data)
        if result == -1 {
            throw MiniAudioErrors.encodeFormatFailed
        }
            
        if result == -2 {
            throw MiniAudioErrors.initDeviceFailed
        }
    }

    public func startAudioCapturing() throws {
        let result = MiniAudioC.startAudioCapturing(&self.ctx)
        if result != 0 {
            throw MiniAudioErrors.capturingAudioFailed
        }
    }

    public func closeCaptureDevice() {
        MiniAudioC.closeCaptureDevice(&self.ctx)
    }
    
    public func getData() -> Data {
        return Data(bytes: self.data.buffer, count: self.data.size)
    }
}

public class AudioPlayer {
    private var ctx: PlaybackContext

    public init(){
        self.ctx = MiniAudioC.PlaybackContext(device: MiniAudioC.ma_device(), audioBuffer: MiniAudioC.ma_audio_buffer(), duration: 0)
    }

    public func initPlaybackDevice(for data: Data) throws {
        var d = data
        try d.withUnsafeMutableBytes { rawBufferPointer in
            let pointer = rawBufferPointer.bindMemory(to: UInt8.self).baseAddress!
            var audioData = MiniAudioC.AudioData(buffer: pointer, size: data.count, offset: 0)
            
            let result = MiniAudioC.initPlackbackDevice(&self.ctx, &audioData)
            if result == -1 || result == -2 {
                throw MiniAudioErrors.decodeFormatFailed
            }
            
            if result == -3 {
                throw MiniAudioErrors.initDeviceFailed
            }
        }
    }
    
    public func getDuration() -> UInt32 {
        return self.ctx.duration
    }
    
    public func startAudioPlaying() throws {
        let result = MiniAudioC.startAudioPlaying(&self.ctx)
        if result != 0 {
            throw MiniAudioErrors.playingAudioFailed
        }
    }
    
    public func updateAudioData(for data: Data) throws {
        var d = data
        try d.withUnsafeMutableBytes { rawBufferPointer in
            let pointer = rawBufferPointer.bindMemory(to: UInt8.self).baseAddress!
            var audioData = MiniAudioC.AudioData(buffer: pointer, size: data.count, offset: 0)
            let result = MiniAudioC.updateAudioData(&self.ctx, &audioData)
            if result != 0 {
                throw MiniAudioErrors.playingAudioFailed
            }
        }
    }
    
    public func stopAudioPlaying() throws {
        let result = MiniAudioC.stopAudioPlaying(&self.ctx)
        if result != 0 {
            throw MiniAudioErrors.playingAudioFailed
        }
    }

    public func closePlaybackDevice() {
        MiniAudioC.closePlaybackDevice(&self.ctx)
    }
}
